#include "http_handler.h"

#include <iostream>
#include <map>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "url/util.h"

std::string trim(std::string str) {
  std::size_t str_start = str.find_first_not_of(" \t\r");
  if (str_start == std::string::npos) {
    return "";
  }
  std::size_t str_end = str.find_last_not_of(" \t\r");
  std::size_t str_length = str_end - str_start + 1;
  return str.substr(str_start, str_length);
}

HttpMethod HttpHandler::read_method(std::string& str) {
  if (str == OPTIONS_STR) {
    return OPTIONS;
  }
  else if (str == GET_STR) {
    return GET;
  }
  else if (str == HEAD_STR) {
    return HEAD;
  }
  else if (str == POST_STR) {
    return POST;
  }
  else if (str == DELETE_STR) {
    return DELETE;
  }
  else if (str == TRACE_STR) {
    return TRACE;
  }
  else if (str == CONNECT_STR) {
    return CONNECT;
  }
  else {
    return UNKNOWN;
  }
}

void HttpHandler::map_and_find_resource(
  //
    HttpRequest& request,
    std::string& www_dir_path,
    HttpResponse& response) {
  // Map root uri to index.html
  if (request.uri == "/") {
    request.uri = "/index.html";
  }
  request.uri = www_dir_path + request.uri;
  std::cout << "Request URI mapped to "
            << request.uri << std::endl;
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
}

void HttpHandler::parse_msg_header(std::stringstream& msg_stream, HttpRequest& request) {
  std::string method, url;
  // Get HTTP method
  msg_stream >> method;
  request.method = this->read_method(method);
  // Get resource URI and query string, if it exists
  msg_stream >> url;
  url = url::decode(url);
  std::size_t query_start = url.find_first_of("?");
  if (query_start == std::string::npos) {
    request.uri = url;
  }
  else {
    request.uri = url.substr(0, query_start);
    request.query = url.substr(query_start + 1);
  }
  // Get HTTP version
  msg_stream >> request.version;
  // Remove remaining CRLF from request-line
  std::getline(msg_stream, url);
  // Get headers fields
  std::map<std::string, std::string> headers;
  std::string header, field_name, field_value;
  std::size_t found;
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
  // To remove remaining CRLF from http header
  std::getline(msg_stream, header);
}

HttpRequest HttpHandler::parse_message(std::string message) {
  std::stringstream msg_stream;
  msg_stream.str(message);
  HttpRequest request;
  parse_msg_header(msg_stream, request);
  // If POST query is in body
  if (request.method == POST) {
    std::string body;
    std::getline(msg_stream, body);
    request.query = url::decode(body);
  }
  return request;
}

HttpResponse HttpHandler::create_response(
  //
    HttpRequest request,
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
    case POST:
      this->map_and_find_resource(request, www_dir_path, response);
      break;
    case OPTIONS:
    case HEAD:
    case DELETE:
    case TRACE:
    case CONNECT:
      response.status = NOT_IMPLEMENTED;
      break;
    case UNKNOWN:
    default:
      response.status = INTERNAL_ERROR;
      break;
  }
  // Pass query to response
  response.query = request.query;
  return response;
}
