/*
 * Summer - /Summer/src/views/DirectFileView.cc
 * author: Massimo Bianchi 2014
 */

#include <summer/views/DirectFileView.h>

#include <summer/Exceptions.h>
#include <summer/logger.h>
#include <summer/mime_types.h>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

namespace summer { namespace views {

DirectFileView::Headers DirectFileView::headers() const {
	path full_path = file;
	Headers h;
	int size = file_size(full_path);
	h["Content-Length"] = lexical_cast<std::string>(size);
	h["Content-Type"] = mime_types::extension_to_type(full_path.extension().c_str());

	return h;
}

string DirectFileView::body() const {
	path full_path = file;

	logger::views.infoStream()
		<< "DirectFileView::body() "
		<< "reading content from "
		<< file;

	string out;
	char buf[512];
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is) {
		logger::views.errorStream() << "DirectFileView::body() " << full_path << " file not found!";
		throw server::exceptions::FileNotFoundException(file);
	}
	while (is.read(buf, sizeof(buf)).gcount() > 0) out += buf;

	return out;
}

}}

