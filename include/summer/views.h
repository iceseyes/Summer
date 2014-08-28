/*
 * views.h
 *
 *  Created on: 28/ago/2014
 *      Author: massimo
 */

#ifndef SUMMER_VIEWS_H_
#define SUMMER_VIEWS_H_

#include <boost/type_traits/add_rvalue_reference.hpp>
#include <sstream>

namespace summer { namespace views {

template<typename T> class ResponseBody {
public:
	typedef T type;
	typedef typename boost::add_rvalue_reference<type>::type typeRef;

	ResponseBody(typeRef body) :
		_body(body) {}

	std::string body() {
		std::stringstream ss;
		ss << _body;
		return ss.str();
	}

private:
	type _body;
};

}}

#endif /* SUMMER_VIEWS_H_ */
