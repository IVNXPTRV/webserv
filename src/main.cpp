
#include <cstdlib>
#include <string>

#include "Server.hpp"

Server server = Server::getServer();
Router router = Server::getRouter();

using namespace std;

int webserv(char *argv[]) {
  string filename = getFilename(argv);

  server.init(filename);
  server.start();
  server.destroy();

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  (void)argc;
  return webserv(argv);
}
