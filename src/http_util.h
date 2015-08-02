#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

// Supported HTTP methods
enum HttpMethod {
  UNKNOWN = 0,
  GET
};

#define GET_STR "GET"

// Supported HTTP versions
#define HTTP_1_1 "HTTP/1.1"

// Supported HTTP status codes
enum HttpStatus {
  OK = 200,
  NOT_FOUND = 404,
  INTERNAL_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  HTTP_VERSION_NOT_SUPPORTED = 505
};

#define OK_STR "200 OK"
#define NOT_FOUND_STR "404 Not Found"
#define INTERNAL_ERROR_STR "505 Internal Server Error"
#define NOT_IMPLEMENTED_STR "501 Not Implemented"
#define HTTP_VERSION_NOT_SUPPORTED_STR "505 HTTP Version not supported"

#define CRLF "\r\n"

#endif  // HTTP_UTIL_H