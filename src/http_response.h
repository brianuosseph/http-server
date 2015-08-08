#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <map>
#include <string>

#include "http_util.h"

class HttpResponse {
public:
  HttpStatus status;
  std::string resource_path;
  std::string query_string;
  std::map<std::string, std::string> query;

  std::string create_message_header();
};

#endif  // HTTP_RESPONSE_H
