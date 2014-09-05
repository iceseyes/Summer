/*
 * Controllers.h
 *
 *  Created on: 27/ago/2014
 *      Author: massimo
 */

#ifndef SUMMER_BASIC_CONTROLLER_H_
#define SUMMER_BASIC_CONTROLLER_H_

#include <summer/data/Model.h>
#include <summer/http/basic.h>
#include <summer/views/ViewResolver.h>

#include <boost/lexical_cast.hpp>

#include <iterator>
#include <set>
#include <string>

namespace summer { namespace controllers {

/// Return a dest value from a string
/// @param out [out] the destination value
/// @param str [in] the value in string format
/// @return out reference
template<typename dest> dest &fromString(dest &out, const std::string &str) {
	out = boost::lexical_cast<dest>(str);
	return out;
}

/// Trait describes struct fields for controller
template<typename Controller> struct Controller_trait {
	using type = Controller;

	template<typename Insertor> static void fields(Insertor begin) {}
	static void set(type &obj, const std::string &param, const std::string &value) {}
	static std::string defaultValue(const std::string &param) { return ""; }
};

}

class RequestHandler {
public:
	using Request = http::Request;
	using Reply = http::Reply;
	using Resolver = views::ViewResolver<Reply>;
	using FieldNames = std::set<std::string>;

	RequestHandler(Resolver &resolver) : processed(false), resolve(resolver) {}
	virtual ~RequestHandler() {}

	virtual void operator()(const Request &request, Reply &rep) = 0;
	virtual FieldNames &fields() const = 0;
	virtual bool match(const Request &request) const = 0;

protected:
	Resolver &resolve;

	template<class Controller> data::enable_if<data::has_model_injector<Controller>(), Controller>&
	inject(Controller &controller, const Request &request, data::Model &model) {
		controller.model(model);
		_inject(controller, request);
		return controller;
	}

	template<class Controller> data::enable_if<!data::has_model_injector<Controller>(), Controller>&
	inject(Controller &controller, const Request &request, data::Model &model) {
		_inject(controller, request);
		return controller;
	}

	template<class Controller>
	Controller &_inject(Controller &controller, const Request &request) {
		using ControllerTrait = controllers::Controller_trait<Controller>;

		for(std::string field : fields<Controller>()) {
			ControllerTrait::set(controller, field, request.parameter(field));
		}

		return controller;
	}

	template<class Controller> FieldNames &fields() const {
		using ControllerTrait = controllers::Controller_trait<Controller>;

		if(!processed) ControllerTrait::fields(std::inserter(_fields, _fields.begin()));
		processed = true;

		return _fields;
	}

	template<class Controller> bool match(const Request &request) const {
		using ControllerTrait = controllers::Controller_trait<Controller>;

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

template<class T> class basic_controller : public RequestHandler {
public:
	using type = T;

	basic_controller(RequestHandler::Resolver &resolver) : RequestHandler(resolver) {}

	virtual void operator()(const Request &request, Reply &rep) {
		apply(request, rep);
	}

	virtual FieldNames &fields() const { return RequestHandler::fields<type>(); }
	virtual bool match(const Request &request) const {
		return RequestHandler::match<type>(request);
	}

private:
	void apply(const Request &request, Reply &rep) {
		data::Model model;
		type controller;
		run(rep, inject(controller, request, model), model);
	}

	template<typename controller>
	data::enable_if<!data::has_model_direct_injector<controller>()>
	run(Reply &rep, controller &c, data::Model &model) {
		resolve(rep, c());
	}

	template<typename controller>
	data::enable_if<data::has_model_direct_injector<controller>()>
	run(Reply &rep, controller &c, data::Model &model) {
		resolve(rep, c(model));
	}
};

}

#endif /* SUMMER_BASIC_CONTROLLER_H_ */
