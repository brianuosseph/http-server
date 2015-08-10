#ifndef URL_UTIL_H
#define URL_UTIL_H

#include <sstream>

namespace url {
  char from_hex(char c) {
    return isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
  }

  char to_hex(char code) {
    static char hex[] = "0123456789abcdef";
    return hex[code & 15];
  }

  bool is_unreserved(char& c) {
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      return true;
    }
    return false;
  }

  std::string encode(std::string url) {
    std::stringstream escaped;
    escaped << std::hex;
    size_t i = 0;
    while (i < url.size()) {
      char c = url[i];
      if (is_unreserved(c)) {
        escaped << c;
      }
      else if (c == ' ') {
        escaped << '+';
      }
      else {
        escaped << '%' << (int) to_hex(c >> 4) << (int) to_hex(c & 15);
      }
      ++i;
    }
    return escaped.str();
  }

  std::string decode(std::string url) {
    std::stringstream ascii;
    size_t i = 0;
    while (i < url.size()) {
      if (url[i] == '%') {
        ascii << (char) (from_hex(url[i+1]) << 4 | from_hex(url[i+2]));
        i += 2;
      }
      else if (url[i] == '+') {
        ascii << ' ';
      }
      else {
        ascii << url[i];
      }
      ++i;
    }
    return ascii.str();
  }
}
#endif  // URL_UTIL_H