/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvoronts <vvoronts@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:46:49 by vvoronts          #+#    #+#             */
/*   Updated: 2025/12/19 16:05:55 by vvoronts         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <map>
#include <cstring>      // memset, strerror
#include <cstdlib>      // exit
#include <unistd.h>     // close, read, write
#include <fcntl.h>      // fcntl, O_NONBLOCK
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // sockaddr_in
#include <poll.h>       // poll, pollfd
#include <cerrno>       // errno

// =============================================================================
// LAYER 1: SOCKET WRAPPER
// Encapsulates the raw OS file descriptor and setup logic.
// =============================================================================
class Socket {
private:
    int _fd;
    int _port;

public:
    Socket(int port) : _port(port) {
        _fd = -1;
    }

    void setup() {
        // 1. Create Socket (IPv4, TCP)
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        if (_fd < 0) throw std::runtime_error("Socket creation failed");

        // 2. Set Options (Reuse Address to avoid "Address already in use")
        int opt = 1;
        if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            throw std::runtime_error("Setsockopt failed");

        // 3. Set Non-Blocking (Crucial for poll loop)
        if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
            throw std::runtime_error("Fcntl failed");

        // 4. Bind to Port
        struct sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY; // Listen on 0.0.0.0
        addr.sin_port = htons(_port);

        if (bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            throw std::runtime_error("Bind failed");

        // 5. Listen
        if (listen(_fd, 100) < 0)
            throw std::runtime_error("Listen failed");

        std::cout << "[Layer 1] Listening on port " << _port << " (FD: " << _fd << ")" << std::endl;
    }

    int getFD() const { return _fd; }
};

// =============================================================================
// LAYER 1: CLIENT WRAPPER
// Represents one active connection.
// =============================================================================
class Client {
private:
    int _fd;

public:
    Client(int fd) : _fd(fd) {}

    int getFD() const { return _fd; }

    // Simulating the read action (In real project, this fills _read_buffer)
    void readData() {
        char buffer[1024];
        ssize_t bytes = recv(_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "[Client " << _fd << "] Received: " << buffer << std::endl;
            // Echo back to prove it works
            send(_fd, "Acknowledged\n", 13, 0);
        } else if (bytes == 0) {
            throw std::runtime_error("Client disconnected");
        } else {
            throw std::runtime_error("Recv error");
        }
    }
};

// =============================================================================
// LAYER 1: SERVER MANAGER (THE ENGINE)
// Manages the poll() loop and dispatches events.
// =============================================================================
class ServerManager {
private:
    std::vector<struct pollfd>  _poll_fds;
    std::map<int, Client*>      _clients;
    std::map<int, Socket*>      _listeners; // Map FD -> Socket*

public:
    ~ServerManager() {
        // Cleanup would go here
    }

    void addListener(int port) {
        Socket* sock = new Socket(port);
        sock->setup();
        
        // Add to Registry
        _listeners[sock->getFD()] = sock;

        // Add to Poll Vector
        struct pollfd pfd;
        pfd.fd = sock->getFD();
        pfd.events = POLLIN; // Monitor for incoming connections
        pfd.revents = 0;
        _poll_fds.push_back(pfd);
    }

    void run() {
        std::cout << "[Layer 1] Server Engine Started (Waiting for events...)" << std::endl;

        while (true) {
            // 1. Wait for events (poll blocks here until something happens)
            if (poll(&_poll_fds[0], _poll_fds.size(), -1) < 0) {
                std::cerr << "Poll failed: " << strerror(errno) << std::endl;
                break;
            }

            // 2. Loop through all FDs to check status
            // Note: iterating backwards allows easy removal of elements if needed
            for (size_t i = 0; i < _poll_fds.size(); ++i) {
                
                // If no event on this FD, continue
                if (_poll_fds[i].revents == 0) continue;

                int fd = _poll_fds[i].fd;

                // CHECK 1: Is it a Listening Socket? (New Connection)
                if (_listeners.count(fd)) {
                     if (_poll_fds[i].revents & POLLIN) {
                        acceptConnection(fd);
                     }
                }
                // CHECK 2: Is it a Client Socket? (Incoming Data)
                else if (_clients.count(fd)) {
                    if (_poll_fds[i].revents & POLLIN) {
                        handleClientMessage(fd, i);
                    }
                }
            }
        }
    }

private:
    void acceptConnection(int listen_fd) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            std::cerr << "Accept failed" << std::endl;
            return;
        }

        // Set non-blocking
        fcntl(client_fd, F_SETFL, O_NONBLOCK);

        // Create Client Object
        Client* new_client = new Client(client_fd);
        _clients[client_fd] = new_client;

        // Add to Poll
        struct pollfd pfd;
        pfd.fd = client_fd;
        pfd.events = POLLIN; // Monitor for reading
        pfd.revents = 0;
        _poll_fds.push_back(pfd);

        std::cout << "[Layer 1] New Connection accepted on FD: " << client_fd << std::endl;
    }

    void handleClientMessage(int fd, size_t index) {
        try {
            _clients[fd]->readData();
        } catch (std::exception &e) {
            // If read fails or client disconnects, clean up
            std::cout << "[Layer 1] Removing Client FD: " << fd << " Reason: " << e.what() << std::endl;
            close(fd);
            delete _clients[fd];
            _clients.erase(fd);
            _poll_fds.erase(_poll_fds.begin() + index);
        }
    }
};

// =============================================================================
// MAIN ENTRY
// =============================================================================
int main() {
    try {
        ServerManager server;
        
        // Setup two ports to prove the multiplexing works
        server.addListener(8080);
        server.addListener(9090);

        server.run();
    } 
    catch (std::exception &e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}