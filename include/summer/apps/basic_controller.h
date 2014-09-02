/*
 * Controllers.h
 *
 *  Created on: 27/ago/2014
 *      Author: massimo
 */

#ifndef SUMMER_BASIC_CONTROLLER_H_
#define SUMMER_BASIC_CONTROLLER_H_

#include <summer/http/basic.h>

#include <iterator>
#include <set>
#include <string>

namespace summer {

template<typename Controller> struct Controller_trait {
	using type = Controller;

	template<typename Insertor> static void fields(Insertor begin) {}
	static void set(type &obj, const std::string &param, const std::string &value) {}
	static std::string defaultValue(const std::string &param) { return ""; }
};

class ApplyController {
public:
	using Request = http::Request;
	using Reply = http::Reply;
	using FieldNames = std::set<std::string>;

	ApplyController() : processed(false) {}
	virtual ~ApplyController() {}

	virtual void operator()(const Request &request, Reply &rep) = 0;
	virtual FieldNames &fields() const = 0;
	virtual bool match(const Request &request) const = 0;

protected:
	template<class Controller>
	Controller &inject(Controller &controller, const Request &request) {
		using ControllerTrait = Controller_trait<Controller>;

		for(std::string field : fields<Controller>()) {
			ControllerTrait::set(controller, field, request.parameter(field));
		}

		return controller;
	}

	template<class Controller> FieldNames &fields() const {
		using ControllerTrait = Controller_trait<Controller>;

		if(!processed) ControllerTrait::fields(std::inserter(_fields, _fields.begin()));
		processed = true;

		return _fields;
	}

	template<class Controller> bool match(const Request &request) const {
		using ControllerTrait = Controller_trait<Controller>;

		for(std::string field : fields<Controller>()) {
			if(request.parameter(field).empty() &&
					ControllerTrait::defaultValue(field).empty())
				return false;
		}

		return true;

	}
private:
	mutable FieldNames 	_fields;
	mutable bool 		processed;
};

template<class T> struct basic_controller : ApplyController {
	using type = T;

	virtual void operator()(const Request &request, Reply &rep) {
		type controller;
		rep.content = inject(controller, request)().body();
	}

	virtual FieldNames &fields() const { return ApplyController::fields<type>(); }
	virtual bool match(const Request &request) const {
		return ApplyController::match<type>(request);
	}
};

}

#endif /* SUMMER_BASIC_CONTROLLER_H_ */
