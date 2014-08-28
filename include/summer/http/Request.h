//
// Request.h
// ~~~~~~~~~~~
//

#ifndef SUMMER_HTTP_REQUEST_HPP
#define SUMMER_HTTP_REQUEST_HPP

#include <summer/http/Header.h>
#include <summer/net/URI.h>

#include <string>
#include <vector>
#include <algorithm>

namespace summer { namespace http {

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

}}

#endif // SUMMER_HTTP_REQUEST_HPP
