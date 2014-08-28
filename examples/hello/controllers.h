/*
 * controllers.h
 *
 *  Created on: 28/ago/2014
 *      Author: massimo
 */

using namespace summer::views;

struct HelloWorldController {
	ResponseBody<std::string> operator()() {
		return std::string("Hello World!");
	}
};

struct HiPostController {
	ResponseBody<std::string> operator()() {
		return std::string("Hello Post!");
	}
};

struct HiGetController {
	ResponseBody<std::string> operator()() {
		return std::string("Hi Get!");
	}
};



