#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>

#include "http_util.h"

// HTTP Request, only supports 
struct HttpRequest {
  HttpMethod method;
  std::string uri;
  std::string version;
};

// Parses socket message to an `HttpRequest`. If the message
// is not a valid HTTP request and error is returned...?
HttpRequest ParseHttpRequest(char* message);

#endif  // HTTP_REQUEST_H
