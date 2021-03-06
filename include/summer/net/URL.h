/*
 * URL.h
 *
 *  Created on: 03/lug/2014
 *      Author: massimo
 */

#ifndef URL_H_
#define URL_H_

#include <summer/logger.h>

#include <summer/net/URI.h>
#include <summer/net/UrlHandler.h>

#include <boost/algorithm/string.hpp>

#include <string>

namespace std {

ostream &operator<<(ostream &out, vector<string> v);

}

namespace summer { namespace net {

template<class _urlHandler = StdUrlHandler>
class basic_url : private _urlHandler {
public:
	typedef _urlHandler UrlHandler;
	typedef std::vector<std::string> PathTokens;

	basic_url(const std::string &url) : _url(url) {}
	basic_url(const URI &uri) {
		_url = uri2url(uri);
	}

	const std::string &url() const { return _url; }
	std::string &url() { return _url; }
	operator std::string&() { return _url; }
	operator const std::string&() const { return _url; }

	std::string application() const {
		PathTokens tokens = pathTokens();
		std::string out;
		if(!tokens.empty())
			out = tokens.at(0);

		logger::net.debugStream() << "URL::resourcePath() url: " << _url << ", Application: " << out;
		return out;
	}

	std::string resourcePath() const {
		PathTokens tokens = pathTokens();

		// delete application part
		if(!tokens.empty()) tokens.erase(tokens.begin());

		std::string out = boost::join(tokens, "/");
		out = out.substr(0, out.find_first_of('?',0));
		logger::net.debugStream() << "URL::resourcePath() url: " << _url << ", resourcePath: " << out;

		return out;
	}

	std::string params() const {
		PathTokens tokens = pathTokens();
		std::string out = boost::join(tokens, "/");
		int pos = out.find_first_of('?',0);
		out = (pos!=std::string::npos) ? out.substr(pos + 1) : "";

		logger::net.debugStream() << "URL::params() url: " << _url << ", params: " << out;

		return out;
	}

	using UrlHandler::uri2url;
private:
	std::string _url;

	PathTokens pathTokens() const {
		using boost::split;
		using boost::is_any_of;
		using boost::token_compress_on;

		PathTokens tokens;
		boost::split(tokens, _url, is_any_of("/"), token_compress_on);

		// check if _url start with /.
		if(!tokens.empty() && tokens[0].empty())
			tokens.erase(tokens.begin());

		logger::net.debugStream() << "URL::pathTokens => tokens " << tokens;

		return tokens;
	}

};

typedef basic_url<> URL;

}}

#endif /* URL_H_ */
