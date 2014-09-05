/*
 * ApplicationRegistry.h
 *
 *  Created on: 26/giu/2014
 *      Author: massimo bianchi
 */

#ifndef SUMMER_APPLICATIONREGISTRY_H_
#define SUMMER_APPLICATIONREGISTRY_H_

#include <summer/Exceptions.h>

#include <summer/data/basic_allocator.h>
#include <summer/http/basic.h>
#include <summer/apps/Application.h>
#include <summer/server/WebAppFolder.h>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <string>
#include <map>

namespace summer { namespace apps {

/// An entry of application register
struct Entry {
	/// Create a new entry named name
	/// @param name [in] key for this entry
	Entry(const std::string &name) : name(name) {}
	virtual ~Entry() {}

	/// Return a new instance of application
	virtual Application *create() const = 0;

	const std::string name;		//!< key of this entry
};

}

/// The registry of all applications
class ApplicationRegistry : private boost::noncopyable {
public:
	using Request 			= http::Request;			//!< Request handled type
	using Applications 		=
			boost::ptr_vector<apps::Entry>;				//!< List of applications
	using iterator			= Applications::iterator;	//!< Iteretor over applications
	using const_iterator	= Applications::const_iterator; //!< Const Iteretor over applications
	using Registry			= std::map<std::string, Application*>; //!< the applications actived

	/// an application registry basic_entry
	/// an implementation of apps::Entry
	template<
		class AppClass,
		template<class> class Allocator = data::basic_allocator>
	struct basic_entry : public apps::Entry, private Allocator<AppClass> {
		typedef AppClass type;		//!< the Application Type

		/// Create a new Application Entry named name
		/// @param name [in] application name
		basic_entry(const std::string &name) : Entry(name) {}

		/// Implements app::Entry::create
		/// @return an instance of this application
		Application *create() const {
			Application *app = static_cast<Application *>(_create());
			app->handle.resolver.appContext =
					server::WebAppFolder::instance().folder() + "/" + name;
			return app;
		}

		using Allocator<type>::_create;
	};

	/// return the instance af the ApplicationRegistry
	static ApplicationRegistry &instance() {
		static ApplicationRegistry _instance;
		return _instance;
	}

	/// Add Application to the registry using Allocator to construct
	/// the Application instance
	/// @param name [in] Application entry name
	/// @return the ApplicationRegistry
	template<class Application, template<class> class Allocator = data::basic_allocator>
	ApplicationRegistry &add(const std::string &name) {
		basic_entry<Application, Allocator> *entry =
				new basic_entry<Application, Allocator>(name);

		applications.push_back(entry);
		return *this;
	}

	/// Get the application instance for request
	/// @param request [in] request to satisfy
	/// @return an instance of Application right for request
	/// @throw ApplicationNotFound if there is not a good Application
	Application &get(const Request &request);

	/// Find a application named name
	iterator find(const std::string &name);

	/// Find a application named name
	const_iterator find(const std::string &name) const;

	/// the first application registred
	iterator begin() { return applications.begin(); }

	/// the last + 1 application registred
	iterator end() { return applications.end(); }

	/// the first application registred
	const_iterator begin() const { return applications.begin(); }

	/// the last + 1 application registred
	const_iterator end() const { return applications.end(); }

private:
	ApplicationRegistry() {}

	Registry 		registry;
	Applications	applications;

	static int sessionCount;
};

}

#endif /* SUMMER_APPLICATIONREGISTRY_H_ */
