/*
 * Application.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <summer/http/basic.h>
#include <summer/apps/basic_controller.h>

#include <string>
#include <memory>

namespace summer {

class Application {
public:
	using Reply 	= http::Reply;
	using Request 	= http::Request;

	virtual ~Application();
	virtual void operator()(const Request &request, Reply &rep) const;

	class _Handle {
		void push(const std::string &resourcePath, const std::string &method = "any");
		void push_all(const std::string &resourcePath = "");

		class _Request {
			using RequestHandlerPtr = std::shared_ptr<RequestHandler>;

			std::string resourcePath;
			std::string method;
			RequestHandlerPtr applier;
			bool catchAll;
			_Handle *handle;

		public:
			_Request(_Handle *h,
				const std::string &resourcePath,
				const std::string &method = "any",
				bool all = false) :
					handle(h), resourcePath(resourcePath),
					method(method), catchAll(all) {}

			template<class Target> _Request &with() {
				applier = RequestHandlerPtr(new basic_controller<Target>(handle->resolver));
				 return *this;
			}

			template<class Target> _Request &with(Target *obj) {
				applier = RequestHandlerPtr(obj);
				 return *this;
			}

			bool empty() const { return !applier; }
			bool match(const Application::Request &request) const;

			bool operator()(const Application::Request &request, Application::Reply &reply) const;
		};

	public:
		using RequestDescriptors 	= std::vector<_Request>;
		using iterator 				= RequestDescriptors::iterator;
		using const_iterator 		= RequestDescriptors::const_iterator;

		_Handle &request(const std::string &resourcePath);
		_Handle &get(const std::string &resourcePath);
		_Handle &post(const std::string &resourcePath);
		_Handle &all(const std::string &resourcePath = "");

		template<class Target> _Handle &with() {
			controllers.back().with<Target>();
			return *this;
		}

		template<class Target> _Handle &with(Target *obj) {
			controllers.back().with(obj);
			return *this;
		}

		void operator()(const Request &request, Reply &reply) const;

		RequestDescriptors controllers;
		RequestHandler::Resolver resolver;
	} handle;
};

}



#endif /* APPLICATION_H_ */
