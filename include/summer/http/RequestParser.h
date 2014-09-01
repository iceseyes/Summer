/*
 * Summer - summer/http/RequestParser.h
 * Author: massimo bianchi 2014
 *
 * Parse a buffer to obtain a Request object
 */
#ifndef SUMMER_HTTP_REQUEST_PARSER_HPP
#define SUMMER_HTTP_REQUEST_PARSER_HPP

#include <summer/logger.h>
#include <summer/http/requestGrammar.h>

#include <boost/tuple/tuple.hpp>
#include <boost/logic/tribool.hpp>

namespace summer { namespace http {

struct Request;

/// Parser for incoming requests.
struct RequestParser {
	RequestParser() {}

	/**
	 * Parses a buffer and setting Request object.
	 *
	 * @param req [out] the request object that corresponds to incoming message.
	 * @param begin [in/out] iterator to begging of the incoming message.
	 * @param end [in] iterator to end of the incoming message.
	 *
	 * @return a tuple with result of parsing (true for success, false for bad request,
	 * indeterminate for more data in requests) and iterator to last processed symbol.
	 */
	template<typename InputIterator>
	boost::tuple<boost::logic::tribool, InputIterator> operator()(Request& req,
			InputIterator begin, InputIterator end) {
		request_grammar<InputIterator> g;
		bool parsed = qi::parse(begin, end, g, req);
		boost::tribool result = boost::indeterminate;

		if(!parsed) result = false;
		else if(begin==end) result = true;
		logger::http.debugStream() << "Parsing request => " << result;

		return boost::make_tuple(result, begin);
	}

};

}}

#endif // SUMMER_HTTP_REQUEST_PARSER_HPP
