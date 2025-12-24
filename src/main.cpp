#include <string>
int webserv(const std::string filename);

int main(const int argc, const char *argv[]) {
  (void)argc;
  return webserv(*(argv + 1));
}
