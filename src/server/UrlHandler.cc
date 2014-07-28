/*
 * UrlHandler.cc
 *
 *  Created on: 01/lug/2014
 *      Author: massimo
 */

#include <net/UrlHandler.h>
#include <net/URL.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace summer::server;

namespace summer { namespace net {

std::string StdUrlHandler::uri2url(const URI &uri) const {
	std::string request_path;
	if (!url_decode(request_path, uri)) {
		throw exceptions::BadRequestException(uri);
	}

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
			|| request_path.find("..") != std::string::npos) {
		throw exceptions::BadRequestException(uri);
	}

	return request_path;
}

bool url_decode(std::string& out, const std::string& in) {
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i) {
		if (in[i] == '%') {
			if (i + 3 <= in.size()) {
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value) {
					out += static_cast<char>(value);
					i += 2;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else if (in[i] == '+') {
			out += ' ';
		} else {
			out += in[i];
		}
	}
	return true;
}

}}


