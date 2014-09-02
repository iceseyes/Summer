/*
 * summer - summer/http/basic.h
 * Author: Massimo Bianchi 2014
 *
 * Defines basic HTTP entities
 */
#ifndef SUMMER_HTTP_BASIC_HTPP
#define SUMMER_HTTP_BASIC_HTPP

#include <summer/net/URI.h>

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <map>
#include <algorithm>

/** \namespace summer::http
 * \brief summer::http namespace is the module of summer library which defines HTTP protocol.
 *
 * This module contains all the structure to define the Summer HTTP server.
 * To achive this
 * 1. Specialize summer::server::Server to handle http::Request and http::Reply.
 * 2. Defines basic HTTP RootDispatcher used by generic protocol to dispatch client requests.
 * 3. Implements Http Header, Request and Reply and algorithms to parsing Http Request (RequestParser).
 */

namespace summer { namespace http {

struct RequestParser;

/// An HTTP Header.
struct Header {
  std::string name;		//!< name of header
  std::string value;	//!< value (as string) of header

  /// An Header is checked by name
  bool operator==(const std::string &name) const { return this->name == name; }
};

/// An HTTP Request.
struct Request {
	using Headers		= std::vector<Header>;			//!< a set of Header
	using URI 			= net::URI;						//!< URI class
	using RequestParser = summer::http::RequestParser;	//!< Parser to use.

	std::string method;			//!< HTTP Method for request
	URI uri;					//!< URI of request
	int http_version_major;		//!< HTTP Version
	int http_version_minor;		//!< HTTP Version
	Headers headers;			//!< Headers
	std::string body;			//!< Request body

	/// Access to header by name. If header doesn't exist create a new one.
	/// @param header [in] header name.
	/// @return the corresponding header value
	std::string &operator[](const std::string &header);

	/// Access to header by name.
	/// @param header [in] header name.
	/// @return the corresponding header value, if header exist. Empty otherwise.
	std::string operator[](const std::string &header) const;

	/// Return the string value of name-parameter.
	/// @return The string value of parameter name if exist, or an empty string otherwise.
	std::string parameter(const std::string &name) const;

private:
	using Parameters = std::map<std::string, std::string>;

	Parameters &params() const;

	mutable Parameters _params;
};

/// An HTTP Reply
struct Reply {
	/// HTTP Status
	enum status_type {
		ok = 200,
		created = 201,
		accepted = 202,
		no_content = 204,
		multiple_choices = 300,
		moved_permanently = 301,
		moved_temporarily = 302,
		not_modified = 304,
		bad_request = 400,
		unauthorized = 401,
		forbidden = 403,
		not_found = 404,
		internal_server_error = 500,
		not_implemented = 501,
		bad_gateway = 502,
		service_unavailable = 503
	} status;

	std::vector<Header> headers;	//!< The headers to be included in the reply.
	std::string content;			//!< The content

	/// makes reply a buffer
	/// @return reply as an ASIO buffer
	std::vector<boost::asio::const_buffer> to_buffers();

	/// Prepare a Stock Reply by type
	/// @param status [in] status to create a reply
	/// @return a reply of status type.
	static Reply stock_reply(status_type status);
};

}}

#endif // SUMMER_HTTP_BASIC_HTPP
