#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http_request.h"
#include "http_response.h"
#include "http_util.h"

class HttpHandler {
public:
  HttpHandler() {};
  ~HttpHandler() {};

  HttpRequest parse_message(char* message);
  HttpResponse create_response(HttpRequest request,
                               std::string& www_dir_path);
};

#endif  // HTTP_HANDLER_H