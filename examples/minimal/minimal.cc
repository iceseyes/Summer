/*
 * Summer - /Summer/examples/minimal/minimal.cc
 * author: Massimo Bianchi 2014
 */

#include <summer/plugin.h>
#include <string>

using namespace std;
using summer::Application;
using summer::views::ResponseBody;

struct controller {
	ResponseBody<string> operator()() {
		return "Hello World";
	}
};

struct MinimalApp : Application {
	MinimalApp() {
		handle.all().with<controller>();
	}
};

SUMMER_APPLICATION(MinimalApp)


