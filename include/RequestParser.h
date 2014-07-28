//
// RequestParser.h
// ~~~~~~~~~~~~~~~~~~
//

#ifndef SUMMER_HTTP_REQUEST_PARSER_HPP
#define SUMMER_HTTP_REQUEST_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>
#include <requestGrammar.h>
#include <logger/logger.h>

namespace summer { namespace server { namespace http {

struct Request;

/// Parser for incoming requests.
class RequestParser {
public:
	RequestParser() : state_(method_start) {}

	void reset() { state_ = method_start; }

	/// Parse some data. The tribool return value is true when a complete request
	/// has been parsed, false if the data is invalid, indeterminate when more
	/// data is required. The InputIterator return value indicates how much of the
	/// input has been consumed.
	template<typename InputIterator>
	boost::tuple<boost::tribool, InputIterator> operator()(Request& req,
			InputIterator begin, InputIterator end) {

		request_grammar<InputIterator> g;
		bool result = qi::parse(begin, end, g) && begin!=end;
		logger::http.debugStream() << "Parsing request => " << result;

		return boost::make_tuple(result, begin);
	}

private:
	boost::tribool consume(Request& req, char input);
	static bool is_char(int c) { return c >= 0 && c <= 127; }
	static bool is_ctl(int c) { return (c >= 0 && c <= 31) || (c == 127); }
	static bool is_tspecial(int c);
	static bool is_digit(int c) { return c >= '0' && c <= '9'; }

	/// The current state of the parser.
	enum state {
		method_start,
		method,
		uri,
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3
	} state_;
};

}}}

#endif // HTTP_SERVER3_REQUEST_PARSER_HPP
