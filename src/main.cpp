#include "web_server.h"

int main() {
  WebServer server = WebServer();
  server.run();
  return 0;
}