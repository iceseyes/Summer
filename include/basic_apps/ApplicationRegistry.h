/*
 * ApplicationRegistry.h
 *
 *  Created on: 26/giu/2014
 *      Author: massimo
 */

#ifndef SUMMER_APPLICATIONREGISTRY_H_
#define SUMMER_APPLICATIONREGISTRY_H_

#include <Exceptions.h>

#include <boost/noncopyable.hpp>
#include <string>
#include <map>
#include <boost/ptr_container/ptr_vector.hpp>
#include <Request.h>

namespace summer {

class Application;

namespace apps {

template<class T>
struct basic_allocator {
	typedef T target_type;
	target_type *_create() const { return new target_type(); }
};

struct Entry {
	Entry(const std::string &name) : name(name) {}
	virtual ~Entry() {}
	virtual Application *create() const = 0;

	const std::string name;
};

}

class ApplicationRegistry : private boost::noncopyable {
public:
	typedef server::http::Request Request;
	typedef boost::ptr_vector<apps::Entry> Applications;
	typedef Applications::iterator			iterator;
	typedef Applications::const_iterator	const_iterator;
	typedef std::map<std::string, Application*>	Registry;

	template<
		class AppClass,
		template<class> class Allocator = apps::basic_allocator>
	struct basic_entry : public apps::Entry, private Allocator<AppClass> {
		typedef AppClass type;

		basic_entry(const std::string &name) : Entry(name) {}
		Application *create() const {
			return static_cast<Application *>(_create());
		}

		using Allocator<type>::_create;
	};

	struct Find {
		Find(const std::string &name) : name(name) {}
		bool operator()(const apps::Entry &entry) const {
			return name == entry.name;
		}

		const std::string name;
	};
	static ApplicationRegistry &instance() {
		static ApplicationRegistry _instance;
		return _instance;
	}

	template<class Application, template<class> class Allocator = apps::basic_allocator>
	ApplicationRegistry &add(const std::string &name) {
		basic_entry<Application, Allocator> *entry =
				new basic_entry<Application, Allocator>(name);

		applications.push_back(entry);
	}

	Application &get(const Request &request);

	iterator find(const std::string &name) {
		return std::find_if(begin(), end(), Find(name));
	}

	const_iterator find(const std::string &name) const {
		return std::find_if(begin(), end(), Find(name));
	}

	iterator begin() { return applications.begin(); }
	iterator end() { return applications.end(); }
	const_iterator begin() const { return applications.begin(); }
	const_iterator end() const { return applications.end(); }
private:
	ApplicationRegistry() {}

	Registry registry;
	Applications	applications;

	static int sessionCount;
};

}

#endif /* SUMMER_APPLICATIONREGISTRY_H_ */
