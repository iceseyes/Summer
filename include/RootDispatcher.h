/*
 * RootDispatcher.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef ROOTDISPATCHER_H_
#define ROOTDISPATCHER_H_

#include <basic_apps/FileSystemApplication.h>
#include <conf/config.h>
#include <basic_apps/ApplicationRegistry.h>
#include <basic_apps/ExceptionApplication.h>
#include <Request.h>

namespace summer { namespace server {

template<class ConfigurationPolicy>
class RootDispatcher {
public:
	typedef http::Request Request;
	typedef ConfigurationPolicy Configuration;
	typedef basic_apps::FileSystemApplication<Configuration> FileSystemApplication;
	typedef basic_apps::ExceptionApplication ExceptionApplication;
	RootDispatcher(const Configuration &config) : fsApp(config) {}

	Application &select(const Request &request) {
		try{
			logger::http.debugStream() << "RootDispatcher::select => searching webapp for " << request.uri;
			return ApplicationRegistry::instance().get(request);
		} catch(exceptions::ApplicationNotFoundException &e) {
			logger::http.debugStream() << "RootDispatcher::select => exception: " << e.what();
			return fsApp;
		} catch (std::exception &e) {
			logger::http.errorStream() << "RootDispatcher::select => exception: " << e.what();
			exApp.message = e.what();
			return exApp;
		}
	}

private:

	FileSystemApplication fsApp; /// the last
	ExceptionApplication exApp;
};

}}



#endif /* ROOTDISPATCHER_H_ */
