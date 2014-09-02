/*
 * RootDispatcher.cc
 *
 *  Created on: 29/ago/2014
 *      Author: massimo
 */

#include <summer/logger.h>

#include <summer/http/basic.h>
#include <summer/http/RootDispatcher.h>

#include <summer/apps/Application.h>
#include <summer/apps/ApplicationRegistry.h>

namespace summer { namespace http {

void RootDispatcher::operator()(const Request &request, Reply &reply) {
	try{
		select(request)(request, reply);
	} catch(server::exceptions::NoControllerDefined &e) {
		logger::server.warnStream()
				<< "http::RootDispatcher() "
				<< "Required controller no defined in application. Trying File System Search for "
				<< request.uri;
		filesystemApplication()(request, reply);
	}
}

void RootDispatcher::badRequest(Reply& reply) {
	reply = http::Reply::stock_reply(http::Reply::bad_request);
}

Application &RootDispatcher::select(const Request &request) {
	Application *app = nullptr;

	try{
		logger::http.debugStream() << "RootDispatcher::select => searching webapp for " << request.uri;
		app = &ApplicationRegistry::instance().get(request);
	} catch(server::exceptions::ApplicationNotFoundException &e) {
		logger::http.debugStream() << "RootDispatcher::select => Application not found: " << e.what();
		app = &filesystemApplication();
	} catch (std::exception &e) {
		logger::http.errorStream() << "RootDispatcher::select => exception: " << e.what();
		app = &exceptionApplication(e.what());
	}

	return *app;
}

}}

