//
// header.h
//

#ifndef SUMMER_HTTP_HEADER_HPP
#define SUMMER_HTTP_HEADER_HPP

#include <string>

namespace summer { namespace http {

struct Header {
  std::string name;
  std::string value;

  bool operator==(const std::string &name) const { return this->name == name; }
};

}}

#endif
