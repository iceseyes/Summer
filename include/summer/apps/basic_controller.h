/*
 * Controllers.h
 *
 *  Created on: 27/ago/2014
 *      Author: massimo
 */

#ifndef SUMMER_BASIC_CONTROLLER_H_
#define SUMMER_BASIC_CONTROLLER_H_

#include <summer/http/basic.h>

namespace summer {

struct ApplyController {
	using Request = http::Request;
	using Reply = http::Reply;

	virtual ~ApplyController() {}
	virtual void operator()(const Request &request, Reply &rep) = 0;
};

template<class T>
struct basic_controller : ApplyController {
	virtual void operator()(const Request &request, Reply &rep) {
		T controller;
		rep.content = controller().body();
	}
};

}

#endif /* SUMMER_BASIC_CONTROLLER_H_ */
