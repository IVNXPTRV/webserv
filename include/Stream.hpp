#ifndef STREAM_HPP
#define STREAM_HPP

class Stream {
 private:
  int _events;
  const int _fd;

 private:
  Stream();

 public:
  virtual void handle() = 0;
};

#endif  // STREAM_HPP
