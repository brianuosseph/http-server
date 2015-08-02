#include "http_handler.h"

// Inteprets request and returns a correct `HttpResponse`
HttpResponse HttpHandler::respond_to(HttpRequest request) {
  HttpResponse response;
  response.resource_path = "";
  // Check if HTTP version is supported
  if (request.version != HTTP_1_1) {
    response.status = HTTP_VERSION_NOT_SUPPORTED;
    return response;
  }
  // Check HTTP method and check for valid file path
  switch (request.method) {
    case GET:
      // bool found = is_valid_path(request.uri);
      if (request.uri == "/") {
        // return index.html
        response.status = OK;
      }
      // else if (request.uri == some_file) {}
      else {
        response.status = NOT_FOUND;
      }
      break;
    case UNKNOWN:
    default:
      response.status = INTERNAL_ERROR;
      break;
  }
  return response;
};
