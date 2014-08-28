/*
 * ExceptionApplication.h
 *
 *  Created on: 03/lug/2014
 *      Author: massimo
 */

#ifndef EXCEPTIONAPPLICATION_H_
#define EXCEPTIONAPPLICATION_H_

#include <summer/apps/Application.h>

namespace summer { namespace apps { namespace basic {

struct ExceptionApplication : public Application {
	std::string message;

	virtual void operator()(const Request &request, Reply &rep) {
		rep.stock_reply(Reply::internal_server_error);
		rep.content = "<html><head><title>500 - Internal Summer Server Error</title></head><body>";
		rep.content += "<h1 style='background-color: orange;'>500 - Internal Server Error</h1>";
		rep.content += "<p>" + message + "</p>";
		rep.content += "</body></html>";
	}
};

}}}



#endif /* EXCEPTIONAPPLICATION_H_ */
