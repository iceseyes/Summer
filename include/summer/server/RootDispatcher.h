/*
 * RootDispatcher.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef ROOTDISPATCHER_H_
#define ROOTDISPATCHER_H_

#include <summer/conf/config.h>
#include <summer/apps/ApplicationRegistry.h>

#include <summer/apps/basic/FileSystemApplication.h>
#include <summer/apps/basic/ExceptionApplication.h>

namespace summer { namespace server {

template<class ConfigurationPolicy, class _Request>
class RootDispatcher {
public:
	typedef _Request Request;
	typedef ConfigurationPolicy Configuration;
	typedef apps::basic::FileSystemApplication<Configuration> FileSystemApplication;
	typedef apps::basic::ExceptionApplication ExceptionApplication;
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
