//
// RequestParser.h
// ~~~~~~~~~~~~~~~~~~
//

#ifndef SUMMER_HTTP_REQUEST_PARSER_HPP
#define SUMMER_HTTP_REQUEST_PARSER_HPP

#include <summer/logger.h>
#include <summer/http/requestGrammar.h>

#include <boost/tuple/tuple.hpp>

namespace summer { namespace http {

struct Request;

/// Parser for incoming requests.
struct RequestParser {
	RequestParser() {}

	template<typename InputIterator>
	boost::tuple<bool, InputIterator> operator()(Request& req,
			InputIterator begin, InputIterator end) {

		request_grammar<InputIterator> g;
		bool result = qi::parse(begin, end, g, req) && begin==end;
		logger::http.debugStream() << "Parsing request => " << result;

		return boost::make_tuple(result, begin);
	}

};

}}

#endif // SUMMER_HTTP_REQUEST_PARSER_HPP
