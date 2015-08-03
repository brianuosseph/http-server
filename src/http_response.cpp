#include "http_response.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string HttpResponse::create_message_header() {
  std::stringstream message_stream;
  // Write status line
  message_stream << HTTP_1_1 << " ";
  switch (status) {
    case OK:
      message_stream << OK_STR << CRLF;
      break;
    case NOT_FOUND:
      message_stream << NOT_FOUND_STR << CRLF;
      break;
    case NOT_IMPLEMENTED:
      message_stream << NOT_IMPLEMENTED_STR << CRLF;
      break;
    case HTTP_VERSION_NOT_SUPPORTED:
      message_stream << HTTP_VERSION_NOT_SUPPORTED_STR << CRLF;
      break;
    // If anything ever goes wrong
    default:
      message_stream << INTERNAL_ERROR_STR << CRLF;
      break;
  }
  // TODO: Check resource_path extension for content type
  // Additional headers would go here.

  // TODO: Persistent connections
  //  - HTTP 1.1 spec says to alway respond with this header
  //    if connection is going to be closed.
  message_stream << "Connection: close" << CRLF;
  // End HTTP header
  message_stream << CRLF;
  return message_stream.str();
}
