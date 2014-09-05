/*
 * Summer - summer/views/ResponseBody.h
 * author: Massimo Bianchi 2014
 *
 * Define a view that indicate only the content.
 */

#ifndef SUMMER_VIEWS_RESPONSEBODY_H_
#define SUMMER_VIEWS_RESPONSEBODY_H_

#include <summer/data/traits.h>
#include <summer/data/serialize_policies.h>

#include <sstream>
#include <string>
#include <map>

namespace summer { namespace views {

/**
 * Create a reply body from user data.
 *
 * A ResponseBody is a view where body of reply obtained from a user data structure.
 * It takes two parameters:
 * 1. the underlying type;
 * 2. the policy for convert underlying type into string.
 */
template<
	typename T,
	template<typename> class SerializePolicy = data::OStreamSerializePolicy>
class ResponseBody : private SerializePolicy<T> {
public:
	using type 			= T;
	using typeRef 		= data::add_reference<type>;
	using typeConstRef 	= data::add_reference<data::add_const<type>>;
	using Headers		= std::map<std::string, std::string>;

	ResponseBody(typeRef body) : _body(body) {}

	template<
		typename ctype =
			data::enable_if<
				!std::is_same<typeRef, typeConstRef>::value,
				typeConstRef>>
	ResponseBody(ctype body) : _body(body) {}

	std::string body() const { return this->toString(_body); }
	Headers headers() const { return _headers; }

private:
	type 		_body;
	Headers		_headers;
};

}}

#endif /* RESPONSEBODY_H_ */
