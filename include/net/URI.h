/*
 * URI.h
 *
 *  Created on: 03/lug/2014
 *      Author: massimo
 */

#ifndef URI_H_
#define URI_H_

#include <string>
#include <iostream>

namespace summer { namespace net {

class URI {
public:
	URI(const std::string &uri="") : _uri(uri) {}

	const std::string &uri() const { return _uri; }
	std::string &uri() { return _uri; }
	operator std::string&() { return uri(); }
	operator const std::string&() const { return uri(); }

private:
	std::string _uri;
};

std::ostream &operator<<(std::ostream &out, const URI &uri);

}}

#endif /* URI_H_ */
