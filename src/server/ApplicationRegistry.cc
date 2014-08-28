/*
 * ApplicationRegistry.cc
 *
 *  Created on: 01/lug/2014
 *      Author: massimo
 */

#include <summer/logger.h>

#include <summer/net/URL.h>

#include <summer/apps/ApplicationRegistry.h>
#include <summer/apps/Application.h>

#include <boost/format.hpp>

namespace summer {

using net::URL;
int ApplicationRegistry::sessionCount = 0;

Application& ApplicationRegistry::get(const Request &request) {
	std::string name = URL(request.uri).application();
	std::string sessionID = request["Cookie"];

	logger::http.debugStream() << "ApplicationRegistry::get trying to find application named: "
			<< name << " Cookies: " << sessionID;

	if(registry.find(sessionID)!=registry.end())
		return *registry[sessionID];

	logger::http.debugStream() << "ApplicationRegistry::get no session found for application named: "
				<< name << " Cookies: " << sessionID;

	iterator iter = find(name);
	if(iter!=end()) {
		std::string sID =
				(boost::format("summer%1%") % ++sessionCount).str();
		registry[sID] = iter->create();
		Application &app = *registry[sID];

		logger::http.debugStream() << "ApplicationRegistry::get Starting new application named: "
						<< name << " Session: " << sID;

		return *registry[sID];
	}

	logger::http.debugStream() << "ApplicationRegistry::get no application found named: "
					<< name << " Cookies: " << sessionID;

	throw server::exceptions::ApplicationNotFoundException(name);
}

}
