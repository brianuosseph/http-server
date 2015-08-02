#include "http_request.h"
#include <iostream>
#include <sstream>

HttpRequest ParseHttpRequest(char* message) {
  std::stringstream msg_stream;
  msg_stream.str(message);
  HttpRequest request;
  std::string method;
  // Get HTTP method
  msg_stream >> method;
  if (method == GET_STR) {
    request.method = GET;
  }
  else {
    request.method = UNKNOWN;
  }
  // Get resource URI
  msg_stream >> request.uri;
  // Get HTTP version
  msg_stream >> request.version;
  // Ignoring all other headers and request body
  return request;
}
