/*
 * requestGrammar.h
 *
 *  Created on: 04/lug/2014
 *      Author: Massimo Bianchi (Gestiware srl)
 */

#ifndef REQUESTGRAMMAR_H_
#define REQUESTGRAMMAR_H_

#undef PARSING_DEBUG

#include <summer/logger.h>
#include <summer/http/Request.h>
#include <summer/Exceptions.h>

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	summer::http::Header,
	(std::string, name)
	(std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
	summer::http::Request,
	(std::string, method)
	(summer::net::URI, uri)
	(int, http_version_major)
	(int, http_version_minor)
	(summer::http::Request::Headers, headers)
	(std::string, body)
)

namespace summer { namespace http {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

/*
HTTP 1.1 Specification

Request = Request-Line
	*(( general-header
		 | request-header
		 | entity-header ) CRLF)
	CRLF
	[ message-body ]

Request-Line   = Method SP Request-URI SP HTTP-Version CRLF

Method         = "OPTIONS"                ; Section 9.2
				  | "GET"                    ; Section 9.3
				  | "HEAD"                   ; Section 9.4
				  | "POST"                   ; Section 9.5
				  | "PUT"                    ; Section 9.6
				  | "DELETE"                 ; Section 9.7
				  | "TRACE"                  ; Section 9.8
				  | "CONNECT"                ; Section 9.9
				  | extension-method
extension-method = token

Request-URI    = "*" | absoluteURI | abs_path | authority

general-header = Cache-Control            ; Section 14.9
			  | Connection               ; Section 14.10
			  | Date                     ; Section 14.18
			  | Pragma                   ; Section 14.32
			  | Trailer                  ; Section 14.40
			  | Transfer-Encoding        ; Section 14.41
			  | Upgrade                  ; Section 14.42
			  | Via                      ; Section 14.45
			  | Warning                  ; Section 14.46

request-header = Accept                   ; Section 14.1
		  | Accept-Charset           ; Section 14.2
		  | Accept-Encoding          ; Section 14.3
		  | Accept-Language          ; Section 14.4
		  | Authorization            ; Section 14.8
		  | Expect                   ; Section 14.20
		  | From                     ; Section 14.22
		  | Host                     ; Section 14.23
		  | If-Match                 ; Section 14.24

		  | If-Modified-Since        ; Section 14.25
		  | If-None-Match            ; Section 14.26
		  | If-Range                 ; Section 14.27
		  | If-Unmodified-Since      ; Section 14.28
		  | Max-Forwards             ; Section 14.31
		  | Proxy-Authorization      ; Section 14.34
		  | Range                    ; Section 14.35
		  | Referer                  ; Section 14.36
		  | TE                       ; Section 14.39
		  | User-Agent               ; Section 14.43

entity-header  = Allow                    ; Section 14.7
			  | Content-Encoding         ; Section 14.11
			  | Content-Language         ; Section 14.12
			  | Content-Length           ; Section 14.13
			  | Content-Location         ; Section 14.14
			  | Content-MD5              ; Section 14.15
			  | Content-Range            ; Section 14.16
			  | Content-Type             ; Section 14.17
			  | Expires                  ; Section 14.21
			  | Last-Modified            ; Section 14.29
			  | extension-header

extension-header = message-header

message-body = entity-body
             | <entity-body encoded as per Transfer-Encoding>

entity-body    = *OCTET

 */
template <typename Iterator>
struct request_grammar : qi::grammar<Iterator, Request()> {
	request_grammar() : request_grammar::base_type(start) {
		using server::exceptions::BadRequestException;
		using qi::eps;
		using qi::uint_;
		using qi::lit;
		using qi::double_;
		using qi::lexeme;
		using qi::on_error;
		using qi::fail;
		using ascii::char_;
		using ascii::digit;
		using ascii::space;
		using namespace qi::labels;

		start %=
			method >> lit(' ') >> uri >> lit(' ') >> lit("HTTP/") >> uint_ >> lit('.') >> uint_ >> CRLF >>
			headers >>
			- ( CRLF >> message_body)
		;

		method %= +(char_ - (space|lit('\n')|lit('\r')));
		uri %= +(char_ - (space|lit('\n')|lit('\r')));

		headers %= *( /* (general_header | request_header | entity_header ) */ simple_header >> CRLF );
		simple_header %= header_name >> lit(": ") >> header_value;
		header_name %= +(char_ - (space|lit('\n')|lit('\r')|lit(':')));
		header_value %= +(char_ - (lit('\n')|lit('\r')));

		message_body %= *char_;

		CRLF = "\r\n";

#ifdef PARSING_DEBUG
		BOOST_SPIRIT_DEBUG_NODE(start);
		BOOST_SPIRIT_DEBUG_NODE(simple_header);
		BOOST_SPIRIT_DEBUG_NODE(headers);
		BOOST_SPIRIT_DEBUG_NODE(header_name);
		BOOST_SPIRIT_DEBUG_NODE(header_value);
		BOOST_SPIRIT_DEBUG_NODE(message_body);
		BOOST_SPIRIT_DEBUG_NODE(method);
		BOOST_SPIRIT_DEBUG_NODE(uri);

		debug(start);
		debug(headers);
		debug(simple_header);
		debug(header_name);
		debug(header_value);
		debug(message_body);
		debug(method);
		debug(uri);
#endif
	}

	qi::rule<Iterator, Request()> start;
	qi::rule<Iterator, std::string()> method;
	qi::rule<Iterator, std::string()> uri;
	qi::rule<Iterator, Header()> simple_header;
	qi::rule<Iterator, Request::Headers()> headers;
	qi::rule<Iterator, std::string()> header_name;
	qi::rule<Iterator, std::string()> header_value;
	qi::rule<Iterator, std::string()> message_body;
	qi::rule<Iterator> CRLF;
};

}}


#endif /* REQUESTGRAMMAR_H_ */
