//
// Request.h
// ~~~~~~~~~~~
//

#ifndef SUMMER_HTTP_REQUEST_HPP
#define SUMMER_HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include "Header.h"
#include <net/URI.h>
#include <algorithm>

namespace summer { namespace server { namespace http {

struct Request {
	typedef std::vector<Header>	Headers;
	typedef net::URI URI;

	std::string method;
	URI uri;
	int http_version_major;
	int http_version_minor;
	Headers headers;
	std::string body;

	std::string &operator[](const std::string &header);
	std::string operator[](const std::string &header) const;
};

}}}

#endif // HTTP_SERVER3_REQUEST_HPP
