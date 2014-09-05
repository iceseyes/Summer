/*
 * controllers.h
 *
 *  Created on: 28/ago/2014
 *      Author: massimo
 */

#include <summer/apps/Controllers.h>

using namespace summer::views;

struct HelloWorldController {
	std::string operator()() { return "hello.html"; }
};

struct HiPostController {
	ResponseBody<std::string> operator()(summer::data::Model &model) {
		return std::string("Hello Post!");
	}
};

struct HiGetController {
	std::string name;
	int age;
	summer::data::Model _model;

	void model(summer::data::Model &m) {
		m["prova"] = 10;
		_model = m;
	}

	ResponseBody<std::string> operator()() {
		std::stringstream ss;
		ss << age;
		ss << _model.get<int>("prova");
		return std::string("Hi ") + name + std::string(" Get! You are ") + ss.str() + " year's old!";
	}
};

SUMMER_CONTROLLER(HiGetController, (name)(age))
