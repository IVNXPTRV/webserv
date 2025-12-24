#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <map>
#include <string>

using namespace std;
typedef struct Key {
  unsigned int ip;
  unsigned short port;
  string name;  // always lowercase
} Key;

class VirtualHost;
typedef map<Key, VirtualHost*> VirtualHosts;

class Router {
 private:
  VirtualHosts _virtualHosts;
};

#endif  // ROUTER_HPP
