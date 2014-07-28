/*
 * Request.cc
 *
 *  Created on: 03/lug/2014
 *      Author: Massimo Bianchi (Gestiware srl)
 */

#include <Request.h>

namespace summer { namespace server { namespace http {

std::string &Request::operator[](const std::string &header) {
	Headers::iterator iter = std::find(headers.begin(), headers.end(), header);
	if(iter!=headers.end())
		return iter->value;

	Header h;
	h.name = header;

	headers.push_back(h);
	return headers.back().value;
}

std::string Request::operator[](const std::string &header) const {
	Headers::const_iterator iter = std::find(headers.begin(), headers.end(), header);
	if(iter!=headers.end())
		return iter->value;

	return "";
}

}}}



