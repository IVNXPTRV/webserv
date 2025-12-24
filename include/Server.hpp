#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <string>

using namespace std;
class Stream;
class Router;

typedef list<Stream*> Streams;

class Server {
 private:
  Router _router;
  Streams _streams;

 private:
  Server();
  void poll();

 public:
  static Server& getServer();
  static Router& getRouter();

  void init(string filename);  // init router and streams
  void start();
  void destroy();  // delete all streams and router
};

#endif  // SERVER_HPP
