#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

#include "http_util.h"

class HttpResponse {
public:
  HttpStatus status;
  std::string resource_path;

  std::string to_string();
};

#endif  // HTTP_RESPONSE_H
