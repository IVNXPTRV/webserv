#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <list>

#include "Connection.hpp"
#include "Processor.hpp"
#include std::list ProcessorsList;

typedef

    class Client : public Connection {
 private:
  ProcessorsList
      _processors;  // attached processros working on behalf of this client
 public:
  Client();
  void handle();
};

#endif  // CLIENT_HPP
