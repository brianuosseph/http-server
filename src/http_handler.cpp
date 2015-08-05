#include "http_handler.h"

#include <iostream>
#include <map>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::string trim(std::string str) {
  std::size_t str_start = str.find_first_not_of(" \t\r");
  if (str_start == std::string::npos) {
    return "";
  }
  std::size_t str_end = str.find_last_not_of(" \t\r");
  std::size_t str_length = str_end - str_start + 1;
  return str.substr(str_start, str_length);
}

HttpRequest HttpHandler::parse_message(std::string message) {
  std::stringstream msg_stream;
  msg_stream.str(message);
  HttpRequest request;
  std::string method;
  // Get HTTP method
  msg_stream >> method;
  if (method == GET_STR) {
    request.method = GET;
  }
  else {
    request.method = UNKNOWN;
  }
  // Get resource URI
  msg_stream >> request.uri;
  // Get HTTP version
  msg_stream >> request.version;
  // Get headers
  std::map<std::string, std::string> headers;
  std::string header, field_name, field_value;
  std::size_t found;
  // To remove remaining CRLF from request-line
  std::getline(msg_stream, header);
  header = "";
  while (true) {
    std::getline(msg_stream, header);
    found = header.find_first_of(":");
    field_name = header.substr(0, found);
    field_value = trim(header.substr(found + 1));
    headers[field_name] = field_value;
    // Check for first char of CRLF, or newline, the end of the request header 
    if (msg_stream.peek() == '\r' || msg_stream.peek() == '\n') {
      break;
    }  
  }
  request.headers = headers;
  // TODO: Support Cookies
  // Save cookies to request
  return request;
}

HttpResponse HttpHandler::create_response(HttpRequest request,
                                          std::string& www_dir_path) {
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
      // Map root uri to index.html
      if (request.uri == "/") {
        request.uri = "/index.html";
      }
      request.uri = www_dir_path + request.uri;
      std::cout << "Request URI mapped to "
                << request.uri << std::endl;
      // TODO: Check if file path is within www_dir_path
      // Check if resource exists 
      struct stat buffer;
      if (stat(request.uri.c_str(), &buffer) != -1
          && S_ISREG(buffer.st_mode)) {
        response.status = OK;
        response.resource_path = request.uri;
      }
      else {
        perror("Error checking for file stats");
        response.status = NOT_FOUND;
      }
      break;
    case UNKNOWN:
    default:
      response.status = INTERNAL_ERROR;
      break;
  }
  // TODO: Support Cookies
  // Parse or save cookies for response
  return response;
}
