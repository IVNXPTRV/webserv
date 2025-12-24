#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace Logger {
enum LogLevel { INFO, WARN, ERR };

// Declaration only - no body, no inline
void log(LogLevel level, const std::string &msg);
}  // namespace Logger

#endif
