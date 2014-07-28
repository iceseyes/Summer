//
// mime_types.h
// ~~~~~~~~~~~~~~
//

#ifndef SUMMER_MIME_TYPES_HPP
#define SUMMER_MIME_TYPES_HPP

#include <string>

namespace summer { namespace server { namespace http { namespace mime_types {

/// Convert a file extension into a MIME type.
std::string extension_to_type(const std::string& extension);

}}}}

#endif // HTTP_SERVER3_MIME_TYPES_HPP
