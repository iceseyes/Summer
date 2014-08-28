/*
 * WebAppFolder.cc
 *
 *  Created on: 28/ago/2014
 *      Author: massimo
 */

#include <summer/logger.h>
#include <summer/server/WebAppFolder.h>

#include <glibmm/module.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>

using namespace std;
using namespace boost::filesystem;
using namespace boost;

namespace summer { namespace server { namespace __impl {

const string Plugin::INIT_FUNCTION_NAME = "initApplication";
const string Plugin::SETUP_FUNCTION_NAME = "setup";
static const std::string dll_extention =
#ifdef BOOST_POSIX_API
		".so";
#else
		".dll";
#endif

using WebAppItem = directory_iterator::value_type;
typedef void (* InitFunc)(const char*);
typedef void (* SetupFunc)();

bool isPlugin(const path &file);

template<class Module, class Function>
bool getFunction(Function &out, const Module &module, const std::string &func) {
	bool ris = false;
	void *tmp;
	if(module->get_symbol(func, tmp)) {
		out = (Function)tmp;
		ris = true;
	}

	return ris;
}

struct CheckModule {
	CheckModule(Dictionary &out) : dictionary(out) {}

	void operator()(const WebAppItem &item) {
		string path { item.path().string() };
		string filename { item.path().filename().string() };

		logger::server.debugStream()
			<< "Processing " << path;

		if(is_directory(path)) {
			directory_iterator iter = find_if(
				directory_iterator(path), directory_iterator(),
				[](const filesystem::path &p) {
					logger::server.debugStream()
							<< "Processing " << p;

					return isPlugin(p);
				});

			if(iter!=directory_iterator()) {
				logger::server.infoStream()
					<< "New module found: " << *iter;
				add(filename, iter->path().string());

			}
		} else if(is_regular_file(path) && isPlugin(path)) {
			logger::server.infoStream()
				<< "New module found: " << path;
			add("", filename);
		}
	}

	void add(const std::string &name, const std::string &library) {
		try{
			auto plugin = Dictionary::value_type(name, library);
			dictionary.insert(plugin);
		} catch (std::logic_error &e) {
			logger::server.errorStream()
				<< "An Exception Occured while loading plugin: "
				<< e.what();
		}
	}

	Dictionary &dictionary;
};

Plugin::Plugin(const std::string &name, const std::string &file) :
		_name(name), _file(file), module(new Glib::Module(file)) {
	init();
}

void Plugin::init() {
	InitFunc init;
	if(getFunction(init, module, INIT_FUNCTION_NAME)) {
		(*init)(_name.c_str());
	} else
		throw server::exceptions::ApplicationNotSupported(_file);
}

void Plugin::setup() {
	SetupFunc setup;
	if(getFunction(setup, module, SETUP_FUNCTION_NAME)) (*setup)();
}

inline bool Plugin::operator==(const Plugin &p) const { return p._name == _name; }
inline bool Plugin::operator<(const Plugin &p) const { return p._name < _name; }

bool isPlugin(const std::string file) noexcept {
	try {
		path p(file);
		return isPlugin(p);
	} catch (...) {}

	return false;
}

bool isPlugin(const path &file) {
	return (to_lower_copy(file.extension().string()) == dll_extention);
}

bool checkWebAppFolder(const string& folder) noexcept {
	try {
		return exists(folder) && is_directory(folder);
	} catch (...) {}

	return false;
}

void scanModules(Dictionary& out, const string& folder) {
	logger::server.infoStream()
			<< "Scanning webapp folder: "
			<< folder << " for modules...";

	CheckModule cm {out};
	for_each(directory_iterator(folder), directory_iterator(), cm);
}

}}}
