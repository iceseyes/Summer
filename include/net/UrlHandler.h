/*
 * UrlValidator.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef URLVALIDATOR_H_
#define URLVALIDATOR_H_

#include <Exceptions.h>

namespace summer { namespace net {

class URI;

bool url_decode(std::string& out, const std::string& in);

struct StdUrlHandler {
	std::string uri2url(const URI &req) const;
};

}}



#endif /* URLVALIDATOR_H_ */
