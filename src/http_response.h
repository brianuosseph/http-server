#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

#include "http_util.h"

// Basic response headers
#define HTTP_OK "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
#define HTTP_BAD "HTTP/1.1 404 Not Found\r\n\r\n"

class HttpResponse {
public:
  HttpStatus status;
  std::string resource_path;

  std::string to_string();
};

#endif  // HTTP_RESPONSE_H
