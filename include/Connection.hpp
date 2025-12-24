#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Stream.hpp"

class Connection : public Stream {
 private:
  const unsigned int ip;      // listen ip
  const unsigned short port;  // listen port

 public:
  Connection();
};

#endif  // CONNECTION_HPP
