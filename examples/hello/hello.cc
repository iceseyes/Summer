/*
 * hello.cc
 *
 *  Created on: 28/ago/2014
 *      Author: massimo
 */

#include <summer/plugin.h>
#include "controllers.h"

using summer::Application;

struct TestApp : Application {
	TestApp() {
		handle
			.request("index.html").with<HelloWorldController>()
			.get("hi.html").with<HiGetController>()
			.post("hi.html").with<HiPostController>();

	}
};

SUMMER_APPLICATION(TestApp)
