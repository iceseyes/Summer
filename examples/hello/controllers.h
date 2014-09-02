/*
 * controllers.h
 *
 *  Created on: 28/ago/2014
 *      Author: massimo
 */

#include <summer/apps/basic_controller.h>

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
	std::string name;

	ResponseBody<std::string> operator()() {
		return std::string("Hi ") + name + std::string(" Get!");
	}
};

namespace summer {

template<> struct Controller_trait<HiGetController> {
	using type = HiGetController;

	template<typename Insertor> static void fields(Insertor begin) {
		begin = "name";
	}

	static void set(type &obj, const std::string &param, const std::string &value) {
		std::string v = value.empty() ? defaultValue(param) : value;
		if(param=="name") obj.name = v;
	}

	static std::string defaultValue(const std::string &param) {
		if(param=="name") return "Massimo";

		return "";
	}
};

}
