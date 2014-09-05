/*
 * Summer - /summer/views/DirectFileView.h
 * author: Massimo Bianchi 2014
 */

#ifndef SUMMER_VIEWS_DIRECTFILEVIEW_H_
#define SUMMER_VIEWS_DIRECTFILEVIEW_H_

#include <string>
#include <map>

namespace summer { namespace views {

/// Take a file as a view
class DirectFileView {
public:
	/// Define a the type for set of headers
	using Headers = std::map<std::string, std::string>;

	/// Create a new direct view that map a file.
	DirectFileView(const std::string &file_path) : file(file_path) {}

	/// Return Reply headers
	Headers headers() const;

	/// Return Reply body;
	std::string body() const;

private:
	std::string file;
};

}}

#endif /* SUMMER_VIEWS_DIRECTFILEVIEW_H_ */
