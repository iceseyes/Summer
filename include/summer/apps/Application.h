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

		struct _Request {
			_Request(const std::string &resourcePath, const std::string &method = "any") :
					resourcePath(resourcePath), method(method) {}

			template<class Target> _Request &with() {
				applier = ApplyControllerPtr(new basic_controller<Target>());
				 return *this;
			}

			template<class Target> _Request &with(Target *obj) {
				applier = ApplyControllerPtr(obj);
				 return *this;
			}

			bool empty() const { return !applier; }
			bool match(const Application::Request &request) const;

			bool operator()(const Application::Request &request, Application::Reply &reply) const;

		private:
			using ApplyControllerPtr = std::shared_ptr<ApplyController>;

			std::string resourcePath;
			std::string method;
			ApplyControllerPtr applier;
		};

	public:
		using RequestDescriptors 	= std::vector<_Request>;
		using iterator 				= RequestDescriptors::iterator;
		using const_iterator 		= RequestDescriptors::const_iterator;

		_Handle &request(const std::string &resourcePath) { push(resourcePath); return *this; }
		_Handle &get(const std::string &resourcePath) { push(resourcePath, "GET"); return *this; }
		_Handle &post(const std::string &resourcePath) { push(resourcePath, "POST"); return *this; }

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

	} handle;
};

}



#endif /* APPLICATION_H_ */
