/*
 * config.h
 *
 *  Created on: 24/giu/2014
 *      Author: massimo
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <boost/utility.hpp>

namespace summer { namespace server { namespace conf {

class Configuration : boost::noncopyable {
public:
	static Configuration &instance() {
		static Configuration _instance;
		return _instance;
	}

	std::string address() const { return _address; }
	std::string port() const { return _port; }
	std::string docRoot() const { return _docRoot; }
	int threadPoolSize() const { return _threadPoolSize; }

private:
	Configuration() :
		_threadPoolSize(5),
		_address("0.0.0.0"), _port("9090"),
		_docRoot("webapps/") {}

	int _threadPoolSize;
	std::string _address;
	std::string _port;
	std::string _docRoot;
};

}}}

#endif /* CONFIG_H_ */
