/*
 * Summer - summer/server/WebAppFolder.h
 * Author: Massimo Bianchi 2013
 *
 * Manage of WebApp Folder
 */
#ifndef SUMMER_SERVER_WEBAPPFOLDER_H_
#define SUMMER_SERVER_WEBAPPFOLDER_H_

#include <summer/Exceptions.h>

#include <summer/conf/config.h>

#include <boost/utility.hpp>

#include <string>
#include <set>
#include <memory>

namespace Glib { class Module; }

namespace summer { namespace server {

namespace __impl {
	/**
	 * Defines a Plugin module.
	 *
	 * A Summer Plugin is a shared library loaded dynamically from a summer server
	 * and used to create a reply to a subset of requests.
	 *
	 * This library must implements a INIT FUNCTION called wher library is loaded
	 * and able to register right class in application registry.
	 *
	 * A SETUP FUNCTION if is founded is loaded to set up any interesting value for library.
	 */
	class Plugin {
	public:
		/// Create a new plugin called name from library founded in file path
		/// @param name [in] the name of new plugin
		/// @param file [in] path of shared library.
		Plugin(const std::string &name, const std::string &file);

		/// compare if two plugins are the same
		bool operator==(const Plugin &p) const;
		/// compare if this plugin name is before p
		bool operator<(const Plugin &p) const;

		/// compare if two plugins are NOT the same
		bool operator!=(const Plugin &p) const { return !(*this == p); }
		/// compare if this plugin is before or the same
		bool operator<=(const Plugin &p) const { return !(*this == p || (*this < p)); }
		/// compare if this plugin name is greater than p
		bool operator>(const Plugin &p) const { return !(*this <= p); }
		/// compare if this plugin name is greater or equal to p
		bool operator>=(const Plugin &p) const { return !(*this < p); }

		/// the basic init function name for plugins
		static const std::string INIT_FUNCTION_NAME;
		/// the setup function name for plugins
		static const std::string SETUP_FUNCTION_NAME;

	private:
		std::string _name;
		std::string _file;
		std::shared_ptr<Glib::Module> module;

		void init();
		void setup();
	};

	/// Define a plugins dictionary
	using Dictionary = std::set<Plugin>;

	/// check for file is a plugin.
	bool isPlugin(const std::string file) noexcept;
	/// explore folder path for plugins
	bool checkWebAppFolder(const std::string &folder) noexcept;
	/// scan folder for plugins and add to dictionary out
	void scanModules(Dictionary &out, const std::string &folder);
}

/// Define a manager for a WebApp Folder.
template<typename C = summer::server::conf::Configuration>
class _WebAppFolder : private boost::noncopyable {
public:
	using Configuration = C;				//!< Configuration to use
	using Dictionary = __impl::Dictionary;	//!< Dictionary to use

	/// Return the unique instance of _WebAppFolder.
	static _WebAppFolder &instance() {
		static _WebAppFolder _instance;
		return _instance;
	}

	/// scan webapp folder and load dictionary
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

/// Define standard WebAppFolder
using WebAppFolder = _WebAppFolder<>;

}}

#endif /* SUMMER_SERVER_WEBAPPFOLDER_H_ */
