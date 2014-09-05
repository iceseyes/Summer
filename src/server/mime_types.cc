//
// mime_types.cpp
// ~~~~~~~~~~~~~~
//

#include <summer/mime_types.h>

namespace summer { namespace mime_types {

struct mapping {
	const char* extension;
	const char* mime_type;
} mappings[] = {
		{ "gif", "image/gif" },
		{ "htm", "text/html" },
		{ "html","text/html" },
		{ "jpg", "image/jpeg" },
		{ "png", "image/png" },
		{ 0, 0 } // Marks end of list.
};

std::string extension_to_type(const std::string& extension) {
	std::string ext = extension;
	if(!extension.empty()&&extension[0]=='.')
		ext = extension.substr(1);
	for (mapping* m = mappings; m->extension; ++m) {
		if (m->extension == ext) {
			return m->mime_type;
		}
	}

	return "text/plain";
}

}}
