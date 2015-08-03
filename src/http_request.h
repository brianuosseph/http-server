#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>

#include "http_util.h"

struct HttpRequest {
  HttpMethod method;
  std::string uri;
  std::string version;
};

#endif  // HTTP_REQUEST_H
