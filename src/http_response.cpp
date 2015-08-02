#include "http_response.h"

#include <sstream>

std::string HttpResponse::to_string() {
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
  // End HTTP header
  message_stream << CRLF;
  // Append body by reading resource_path file
  message_stream << "<html><body><h1>Hello, World!</h1></body></html>";
  return message_stream.str();
}
