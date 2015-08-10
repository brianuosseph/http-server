#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http_request.h"
#include "http_response.h"
#include "http_util.h"

class HttpHandler {
public:
  HttpHandler() {};
  ~HttpHandler() {};

  HttpRequest parse_message(std::string message);
  HttpResponse create_response(HttpRequest request,
                               std::string& www_dir_path);
private:
  HttpMethod read_method(std::string& str);
  void map_and_find_resource(HttpRequest& request,
                             std::string& www_dir_path,
                             HttpResponse& response);
};

std::string trim(std::string& str);

#endif  // HTTP_HANDLER_H