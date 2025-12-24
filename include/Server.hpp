#ifndef SERVER_HPP
#define SERVER_HPP

#include <Router.hpp>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Stream;
typedef map<Stream*, Stream*> StreamsMap;
typedef vector<Stream*> StreamsBatch;

class Server {
 private:
  Router _router;
  StreamsMap _streams;

 private:
  Server();
  StreamsBatch
  poll();  // return set of streams with events in it; read, write event

 public:
  static Server& getServer();
  static Router& getRouter();

  void init(const string filename);  // init router and streams
  void addStream(const Stream*);
  void deleteStream(const Stream*);
  void start();
  void destroy();  // delete all streams and router
};

#endif  // SERVER_HPP
