#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

// HTTP request methods
enum HttpMethod {
  UNKNOWN = 0,
  OPTIONS,
  GET,
  HEAD,
  POST,
  DELETE,
  TRACE,
  CONNECT
};

#define OPTIONS_STR "OPTIONS"
#define GET_STR "GET"
#define HEAD_STR "HEAD"
#define POST_STR "POST"
#define DELETE_STR "DELETE"
#define TRACE_STR "TRACE"
#define CONNECT_STR "CONNECT"

// Supported HTTP versions
#define HTTP_1_1 "HTTP/1.1"

// Supported HTTP status codes
enum HttpStatus {
  // Successful
  OK = 200,
  // Client Error
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  // Server Error
  INTERNAL_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  HTTP_VERSION_NOT_SUPPORTED = 505
};

#define OK_STR "200 OK"
#define FORBIDDEN_STR "403 Forbidden"
#define NOT_FOUND_STR "404 Not Found"
#define INTERNAL_ERROR_STR "505 Internal Server Error"
#define NOT_IMPLEMENTED_STR "501 Not Implemented"
#define HTTP_VERSION_NOT_SUPPORTED_STR "505 HTTP Version not supported"

#define CRLF "\r\n"

#endif  // HTTP_UTIL_H