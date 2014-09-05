//
// reply.cpp
// ~~~~~~~~~
//

#include <summer/logger.h>

#include <summer/http/basic.h>

#include <summer/net/URL.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <string>

using namespace std;
using namespace boost;

namespace summer { namespace http {

namespace status_strings {

const string ok = "HTTP/1.0 200 OK\r\n";
const string created = "HTTP/1.0 201 Created\r\n";
const string accepted = "HTTP/1.0 202 Accepted\r\n";
const string no_content = "HTTP/1.0 204 No Content\r\n";
const string multiple_choices = "HTTP/1.0 300 Multiple Choices\r\n";
const string moved_permanently = "HTTP/1.0 301 Moved Permanently\r\n";
const string moved_temporarily = "HTTP/1.0 302 Moved Temporarily\r\n";
const string not_modified = "HTTP/1.0 304 Not Modified\r\n";
const string bad_request = 		 "HTTP/1.0 400 Bad Request\r\n";
const string unauthorized = 		 "HTTP/1.0 401 Unauthorized\r\n";
const string forbidden = 			 "HTTP/1.0 403 Forbidden\r\n";
const string not_found = 			 "HTTP/1.0 404 Not Found\r\n";
const string internal_server_error ="HTTP/1.0 500 Internal Server Error\r\n";
const string not_implemented = 	"HTTP/1.0 501 Not Implemented\r\n";
const string bad_gateway = 		"HTTP/1.0 502 Bad Gateway\r\n";
const string service_unavailable = "HTTP/1.0 503 Service Unavailable\r\n";

asio::const_buffer to_buffer(Reply::status_type status) {
	switch (status) {
	case Reply::ok: return asio::buffer(ok); break;
	case Reply::created: return asio::buffer(created); break;
	case Reply::accepted: return asio::buffer(accepted); break;
	case Reply::no_content: return asio::buffer(no_content); break;
	case Reply::multiple_choices: return asio::buffer(multiple_choices); break;
	case Reply::moved_permanently:
		return asio::buffer(moved_permanently);
	case Reply::moved_temporarily:
		return asio::buffer(moved_temporarily);
	case Reply::not_modified:
		return asio::buffer(not_modified);
	case Reply::bad_request:
		return asio::buffer(bad_request);
	case Reply::unauthorized:
		return asio::buffer(unauthorized);
	case Reply::forbidden:
		return asio::buffer(forbidden);
	case Reply::not_found:
		return asio::buffer(not_found);
	case Reply::internal_server_error:
		return asio::buffer(internal_server_error);
	case Reply::not_implemented:
		return asio::buffer(not_implemented);
	case Reply::bad_gateway:
		return asio::buffer(bad_gateway);
	case Reply::service_unavailable:
		return asio::buffer(service_unavailable);
	default:
		return asio::buffer(internal_server_error);
	}
}

} // namespace status_strings

namespace stock_replies {

const char ok[] = "";
const char created[] =
  "<html>"
  "<head><title>Created</title></head>"
  "<body><h1>201 Created</h1></body>"
  "</html>";
const char accepted[] =
  "<html>"
  "<head><title>Accepted</title></head>"
  "<body><h1>202 Accepted</h1></body>"
  "</html>";
const char no_content[] =
  "<html>"
  "<head><title>No Content</title></head>"
  "<body><h1>204 Content</h1></body>"
  "</html>";
const char multiple_choices[] =
  "<html>"
  "<head><title>Multiple Choices</title></head>"
  "<body><h1>300 Multiple Choices</h1></body>"
  "</html>";
const char moved_permanently[] =
  "<html>"
  "<head><title>Moved Permanently</title></head>"
  "<body><h1>301 Moved Permanently</h1></body>"
  "</html>";
const char moved_temporarily[] =
  "<html>"
  "<head><title>Moved Temporarily</title></head>"
  "<body><h1>302 Moved Temporarily</h1></body>"
  "</html>";
const char not_modified[] =
  "<html>"
  "<head><title>Not Modified</title></head>"
  "<body><h1>304 Not Modified</h1></body>"
  "</html>";
const char bad_request[] =
  "<html>"
  "<head><title>Bad Request</title></head>"
  "<body><h1>400 Bad Request</h1></body>"
  "</html>";
const char unauthorized[] =
  "<html>"
  "<head><title>Unauthorized</title></head>"
  "<body><h1>401 Unauthorized</h1></body>"
  "</html>";
const char forbidden[] =
  "<html>"
  "<head><title>Forbidden</title></head>"
  "<body><h1>403 Forbidden</h1></body>"
  "</html>";
const char not_found[] =
  "<html>"
  "<head><title>Not Found</title></head>"
  "<body><h1>404 Not Found</h1></body>"
  "</html>";
const char internal_server_error[] =
  "<html>"
  "<head><title>Internal Server Error</title></head>"
  "<body><h1>500 Internal Server Error</h1></body>"
  "</html>";
const char not_implemented[] =
  "<html>"
  "<head><title>Not Implemented</title></head>"
  "<body><h1>501 Not Implemented</h1></body>"
  "</html>";
const char bad_gateway[] =
  "<html>"
  "<head><title>Bad Gateway</title></head>"
  "<body><h1>502 Bad Gateway</h1></body>"
  "</html>";
const char service_unavailable[] =
  "<html>"
  "<head><title>Service Unavailable</title></head>"
  "<body><h1>503 Service Unavailable</h1></body>"
  "</html>";

string to_string(Reply::status_type status) {
	switch (status) {
	case Reply::ok:
		return ok;
	case Reply::created:
		return created;
	case Reply::accepted:
		return accepted;
	case Reply::no_content:
		return no_content;
	case Reply::multiple_choices:
		return multiple_choices;
	case Reply::moved_permanently:
		return moved_permanently;
	case Reply::moved_temporarily:
		return moved_temporarily;
	case Reply::not_modified:
		return not_modified;
	case Reply::bad_request:
		return bad_request;
	case Reply::unauthorized:
		return unauthorized;
	case Reply::forbidden:
		return forbidden;
	case Reply::not_found:
		return not_found;
	case Reply::internal_server_error:
		return internal_server_error;
	case Reply::not_implemented:
		return not_implemented;
	case Reply::bad_gateway:
		return bad_gateway;
	case Reply::service_unavailable:
		return service_unavailable;
	default:
		return internal_server_error;
	}
}

}

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

string &Request::operator[](const string &header) {
	Headers::iterator iter = find(headers.begin(), headers.end(), header);
	if(iter!=headers.end())
		return iter->value;

	Header h;
	h.name = header;

	headers.push_back(h);
	return headers.back().value;
}

string Request::operator[](const string &header) const {
	Headers::const_iterator iter = find(headers.begin(), headers.end(), header);
	if(iter!=headers.end())
		return iter->value;

	return "";
}

string Request::parameter(const string& name) const {
	Parameters::const_iterator iter = params().find(name);
	if(iter!=params().end())
		return iter->second;

	return "";
}

Request::Parameters &Request::params() const {
	using boost::split;
	using boost::is_any_of;
	using boost::token_compress_on;
	using Tokens = vector<string>;

	if(_params.size()==0) {
		net::URL url {uri};
		Tokens tokens;

		string params { url.params() };
		if(params.size()>0) params += "&";
		params += body;

		boost::split(tokens, params, is_any_of("&"), token_compress_on);

		for(string token : tokens) {
			if(token.empty()) continue;

			logger::http.debugStream()
				<< "Request::params(): "
				<< "processing request body token '"
				<< token << "' as parameter pair...";

			Tokens nv_pair;
			boost::split(nv_pair, token, is_any_of("="), token_compress_on);

			int ntok = nv_pair.size();
			switch(ntok) {
			case 0:
			case 1:
				logger::http.debugStream()
					<< "Request::params(): "
					<< "'" << token << "'" << " no value found.";
				_params[token] = "";
				break;
			default:
				logger::http.debugStream()
					<< "Request::params(): "
					<< "'" << token << "'" << " make parameter '"
					<< nv_pair[0] << "' with value '" << nv_pair[1] << "'.";
				_params[nv_pair[0]] = nv_pair[1];
				break;
			}
		}
	}

	return _params;
}

vector<asio::const_buffer> Reply::to_buffers() {
	vector<asio::const_buffer> buffers;
	buffers.push_back(status_strings::to_buffer(status));
	for (size_t i = 0; i < headers.size(); ++i) {
		Header& h = headers[i];
		buffers.push_back(asio::buffer(h.name));
		buffers.push_back(
				asio::buffer(misc_strings::name_value_separator));
		buffers.push_back(asio::buffer(h.value));
		buffers.push_back(asio::buffer(misc_strings::crlf));
	}

	buffers.push_back(asio::buffer(misc_strings::crlf));
	buffers.push_back(asio::buffer(content));
	return buffers;
}

Reply Reply::stock_reply(Reply::status_type status) {
  Reply rep;
  rep.status = status;
  rep.content = stock_replies::to_string(status);
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = lexical_cast<string>(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/html";
  return rep;
}

inline bool Header::operator==(const std::string &name) const { return this->name == name; }

Header &Header::operator=(const std::pair<std::string, std::string> &p) {
	name = p.first; value = p.second;
	return *this;
}

Header::operator std::pair<std::string, std::string>() const {
	return std::pair<std::string, std::string> {name, value};
}
}}
