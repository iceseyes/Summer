/*
 * FileSystemApplication.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef FILESYSTEMAPPLICATION_H_
#define FILESYSTEMAPPLICATION_H_

#include <summer/logger.h>
#include <summer/mime_types.h>

#include <summer/net/URL.h>
#include <summer/apps/Application.h>

#include <boost/lexical_cast.hpp>

#include <fstream>

namespace summer { namespace apps { namespace basic {

template<class ConfigurationPolicy>
class FileSystemApplication : public Application {
public:
	typedef net::URL URL;
	typedef ConfigurationPolicy Configuration;

	FileSystemApplication(const Configuration &config) :
		_docRoot(config.docRoot()) {}

	virtual void operator()(const Request &request, Reply &rep) const {
		logger::server.infoStream()
				<< "No Application Found, trying file system searching...";

		URL request_url(request.uri);
		std::string &request_path = request_url;

		// If path ends in slash (i.e. is a directory) then add "index.html".
		if (request_path[request_path.size() - 1] == '/') {
			request_path += "index.html";
		}

		// Determine the file extension.
		std::size_t last_slash_pos = request_path.find_last_of("/");
		std::size_t last_dot_pos = request_path.find_last_of(".");
		std::string extension;
		if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
			extension = request_path.substr(last_dot_pos + 1);
		}

		// Open the file to send back.
		std::string full_path = _docRoot + request_path;
		std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
		if (!is) {
			logger::http.errorStream() << "FileSystemApplication(): " << full_path << " file not found!";
			rep = Reply::stock_reply(Reply::not_found);
			return;
		}

		// Fill out the reply to be sent to the client.
		rep.status = Reply::ok;
		char buf[512];
		while (is.read(buf, sizeof(buf)).gcount() > 0)
			rep.content.append(buf, is.gcount());
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type(extension);

		logger::http.infoStream() << "FileSystemApplication(): " <<
				request_path << " accepted! "
				<< full_path << " replied";

	}

private:
	std::string _docRoot;
};

}}}

#endif /* FILESYSTEMAPPLICATION_H_ */
