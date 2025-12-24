#include "Logger.hpp"

#include <ctime>
#include <iostream>

namespace Logger {
void log(LogLevel level, const std::string& msg) {
  std::time_t now = std::time(0);
  char buf[20];

  // Use std::localtime and std::strftime from <ctime>
  std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));

  if (level == ERR) {
    std::cerr << "[" << buf << "] [ERR] " << msg << std::endl;
  } else {
    const char* levelStr = (level == INFO) ? "INFO" : "WARN";
    std::cout << "[" << buf << "] [" << levelStr << "] " << msg << std::endl;
  }
}
}  // namespace Logger
