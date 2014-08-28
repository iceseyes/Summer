/*
 * WebAppFolder.h
 *
 *  Created on: 28/ago/2014
 *      Author: massimo
 */

#ifndef WEBAPPFOLDER_H_
#define WEBAPPFOLDER_H_

#include <summer/conf/config.h>
#include <summer/Exceptions.h>

#include <boost/utility.hpp>

#include <string>
#include <set>
#include <memory>

namespace Glib { class Module; }

namespace summer { namespace server {

namespace __impl {
	class Plugin {
	public:
		Plugin(const std::string &name, const std::string &file);

		bool operator==(const Plugin &p) const;
		bool operator<(const Plugin &p) const;

		bool operator!=(const Plugin &p) const { return !(*this == p); }
		bool operator<=(const Plugin &p) const { return !(*this == p || (*this < p)); }
		bool operator>(const Plugin &p) const { return !(*this <= p); }
		bool operator>=(const Plugin &p) const { return !(*this < p); }

		static const std::string INIT_FUNCTION_NAME;
		static const std::string SETUP_FUNCTION_NAME;
	private:
		std::string _name;
		std::string _file;
		std::shared_ptr<Glib::Module> module;

		void init();
		void setup();
	};

	using Dictionary = std::set<Plugin>;

	bool isPlugin(const std::string file) noexcept;
	bool checkWebAppFolder(const std::string &folder) noexcept;
	void scanModules(Dictionary &out, const std::string &folder);
}

template<typename C = summer::server::conf::Configuration>
class _WebAppFolder : private boost::noncopyable {
public:
	using Configuration = C;
	using Dictionary = __impl::Dictionary;

	static _WebAppFolder &instance() {
		static _WebAppFolder _instance;
		return _instance;
	}

	_WebAppFolder &scan() {
		using namespace __impl;
		using namespace server::exceptions;
		using namespace std;

		string folder { Configuration::instance().docRoot() };

		if(!checkWebAppFolder(folder))
			throw WebAppFolderDoesNotExist(folder);

		scanModules(modules, folder);

		return *this;
	}
private:
	_WebAppFolder() {}

	Dictionary modules;
};

using WebAppFolder = _WebAppFolder<>;

}}



#endif /* WEBAPPFOLDER_H_ */
