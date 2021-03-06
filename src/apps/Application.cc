/*
 * Application.cc
 *
 *  Created on: 27/ago/2014
 *      Author: massimo
 */

#include <summer/Exceptions.h>

#include <summer/net/URL.h>
#include <summer/data/Model.h>
#include <summer/apps/Application.h>

#include <boost/algorithm/string.hpp>

#include <algorithm>

using namespace std;
using summer::net::URL;
using summer::server::exceptions::NoControllerDefined;

namespace summer {

bool Application::_Handle::_Request::match(const Application::Request &request) const {
	if(method.empty() || method == "any" || method == boost::to_upper_copy(request.method)) {
		bool nameMatch = catchAll && resourcePath.empty();
		if(!nameMatch) {
			URL url(request.uri);	// parsing uri
			if(catchAll)
				nameMatch = url.resourcePath().find(resourcePath) == 0;
			else
				nameMatch = boost::trim_copy(resourcePath) == boost::trim_copy(url.resourcePath());
		}
		return nameMatch && applier->match(request);
	}

	return false;
}

bool Application::_Handle::_Request::operator()(
		const Application::Request &request, Application::Reply &reply) const {
	if(!empty() && match(request)) {
		(*applier)(request, reply);
		return true;
	}

	return false;
}

Application::~Application() {}

void Application::operator()(const Request &request, Reply &rep) const {
	handle(request, rep);
}

void Application::_Handle::operator()(const Request &request, Reply &reply) const {
	const_iterator iter = find_if(controllers.begin(), controllers.end(),
			[&request, &reply](const _Request &r){
				return r(request, reply);
			});

	if(iter==controllers.end()) {
		URL url(request.uri);
		throw NoControllerDefined(url.application(), url.resourcePath());
	}
}

Application::_Handle &Application::_Handle::request(const std::string &resourcePath) { push(resourcePath); return *this; }
Application::_Handle &Application::_Handle::get(const std::string &resourcePath) { push(resourcePath, "GET"); return *this; }
Application::_Handle &Application::_Handle::post(const std::string &resourcePath) { push(resourcePath, "POST"); return *this; }
Application::_Handle &Application::_Handle::all(const std::string &resourcePath) { push_all(resourcePath); return *this; }

void Application::_Handle::push(
		const std::string& resourcePath,
		const std::string& method) {
	controllers.push_back(_Request(this, resourcePath, method));
}

void Application::_Handle::push_all(const std::string& resourcePath) {
	controllers.push_back(_Request(this, resourcePath, "any", true));
}

}
