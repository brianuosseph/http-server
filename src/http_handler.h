#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http_request.h"
#include "http_response.h"
#include "http_util.h"

class HttpHandler {
public:
  HttpHandler() {};
  ~HttpHandler() {};
  HttpResponse respond_to(HttpRequest request,
                          std::string& www_dir_path);
};

#endif  // HTTP_HANDLER_H