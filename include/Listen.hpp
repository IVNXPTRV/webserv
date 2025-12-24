#ifndef LISTEN_HPP
#define LISTEN_HPP

#include "Connection.hpp"

class Listen : public Connection {
 private:
 public:
  Listen();
  void handle();
};

#endif  // LISTEN_HPP
