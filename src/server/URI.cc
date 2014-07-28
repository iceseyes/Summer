/*
 * URI.cc
 *
 *  Created on: 03/lug/2014
 *      Author: Massimo Bianchi (Gestiware srl)
 */

#include <net/URI.h>

namespace summer { namespace net {

std::ostream &operator<<(std::ostream &out, const URI &uri) {
	out << uri.uri();
	return out;
}

}}


