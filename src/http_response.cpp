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
    case FORBIDDEN:
      message_stream << FORBIDDEN_STR << CRLF;
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
  // TODO: Header fields
  //    - Use bitmask in the respone to keep track of which
  //      ones to write to the stringstream.

  // Quick implementation of Content-Type header
  // NOTE: Refractor to use a library for external file
  //       for getting the MIME type.
  std::size_t found = resource_path.find_last_of(".");
  std::string extension = resource_path.substr(found + 1);
  message_stream << "Content-Type: ";
  // Sompe MIME types off the top of my head.
  if (extension == "html" | extension == "css" |
      extension == "") {
    message_stream << "text/" << extension << CRLF;
  }
  else if (extension == "js" | extension == "json") {
    message_stream << "application/" << extension << CRLF;
  }
  else if (extension == "png" | extension == "tiff" |
           extension == "gif" | extension == "webp" |
           extension == "bmp") {
    message_stream << "image/" << extension << CRLF;
  }
  else if (extension == "jpg" | extension == "jpeg") {
    message_stream << "image/jpeg" << CRLF;
  }
  else {
    message_stream << "text/plain" << CRLF;
  }
  // TODO: Persistent connections
  //  - HTTP 1.1 spec says to alway respond with this header
  //    if connection is going to be closed.
  message_stream << "Connection: close" << CRLF;
  // End HTTP header
  message_stream << CRLF;
  return message_stream.str();
}

