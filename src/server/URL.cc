/*
 * URL.cc
 *
 *  Created on: 03/lug/2014
 *      Author: Massimo Bianchi (Gestiware srl)
 */

#include <summer/net/URL.h>

namespace std {

ostream &operator<<(ostream &out, vector<string> v) {
	out << "[";
	bool first = true;
	for(string s : v) {
		if(!first) out << ", ";
		out << s;

		first = false;
	}
	out << "]";
	return out;
}

}
