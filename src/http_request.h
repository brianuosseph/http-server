#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <map>

#include "http_util.h"

struct HttpRequest {
  HttpMethod method;
  std::string uri;
  std::string query;
  std::string version;
  std::map<std::string, std::string> headers;
};

#endif  // HTTP_REQUEST_H
