/*
 * Application.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <Reply.h>
#include <Request.h>

namespace summer {

class Application {
public:
	typedef server::http::Reply Reply;
	typedef server::http::Request Request;

	virtual ~Application() {}

	virtual void operator()(const Request &request, Reply &rep) = 0;
};
}



#endif /* APPLICATION_H_ */
