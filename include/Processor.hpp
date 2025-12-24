#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "Client.hpp"
#include "Request.hpp"
#include "Stream.hpp"

class Processor : public Stream {
 private:
  const Client& _client;
  const Request& _request;

 public:
  Processor();
  void handle();
};

#endif  // PROCESSOR_HPP
