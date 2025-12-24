#include <cstdlib>
#include <ctime>
#include <string>

#include "Error.hpp"
#include "Logger.hpp"
#include "Server.hpp"

using namespace std;
using namespace Logger;

Server server = Server::getServer();
Router router = Server::getRouter();

int webserv(const string filename) {
  try {
    server.init(filename);
    server.start();
    server.destroy();
    return EXIT_SUCCESS;
  } catch (error err) {
    log(ERR, err);
    server.destroy();
    return EXIT_FAILURE;
  }
}
