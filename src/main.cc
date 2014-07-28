//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <Server.h>
#include <conf/config.h>
#include <logger/logger.h>
#include <basic_apps/ApplicationRegistry.h>
#include <boost/type_traits/add_rvalue_reference.hpp>
#include <sstream>
#include <net/URL.h>

using namespace summer::server;

using namespace summer::server::http;

template<typename T>
class ResponseBody {
public:
	typedef T type;
	typedef typename boost::add_rvalue_reference<type>::type typeRef;

	ResponseBody(typeRef body) :
		_body(body) {}

	std::string body() {
		std::stringstream ss;
		ss << _body;
		return ss.str();
	}

private:
	type _body;
};


struct HelloWorldController {
	ResponseBody<std::string> operator()() {
		return std::string("Mandi Mond");
	}
};

struct ApplyController {
	virtual ~ApplyController() {}
	virtual void operator()(const Request &request, Reply &rep) = 0;
};

template<class T>
struct basic_controller : ApplyController {
	virtual void operator()(const Request &request, Reply &rep) {
		T controller;
		rep.content = controller().body();
	}
};

struct TestApp : summer::Application {
	TestApp() {
		handle
			.request
				.of("index.html")
				.method("GET")
				.with<HelloWorldController>()
			.and_request()
				.of("hello.html")
				.method("POST")
				.with<HelloWorldController>();
	}

	void operator()(const Request &request, Reply &rep) {
		handle(request, rep);
	}

	struct _Handle {
		struct _Request {
			_Request() : applier(nullptr), _request(nullptr) {}
			~_Request() { delete applier; delete _request; }
			_Request &of(const std::string &resourcePath) {
				this->resourcePath = resourcePath;
				return *this;
			}
			_Request &method(const std::string &_method) {
				 this->_method = _method;
				 return *this;
			}
			template<class Target>
			_Request &with() {
				applier = new basic_controller<Target>();
				 return *this;
			}
			_Request &and_request() {
				_request = new _Request();
				return *_request;
			}

			bool empty() const { return applier==nullptr; }
			bool match(const Request &request) const {
				using summer::net::URL;
				URL url = request.uri;

				return boost::trim_copy(resourcePath) == boost::trim_copy(url.resourcePath());
			}

			void operator()(const Request &request, Reply &reply) {
				if(!empty() && match(request)) {
					(*applier)(request, reply);
					return;
				}

				if(_request!=nullptr) {
					(*this->_request)(request, reply);
				} else {
					using summer::net::URL;
					URL url = request.uri;
					throw summer::server::exceptions::NoControllerDefined(
							url.application(), url.resourcePath());
				}
			}
		private:
			std::string resourcePath;
			std::string _method;
			ApplyController *applier;
			_Request *_request;

		} request;

		void operator()(const Request &request, Reply &reply) {
			this->request(request, reply);
		}
	} handle;
};

int main(int argc, char* argv[]) {
  try {
	  summer::logger::initLogger();
	  summer::ApplicationRegistry::instance().add<TestApp>("test");

	  StdServer s;
	  s.run();
  } catch (std::exception& e) {
	  summer::logger::http.errorStream() << "exception: " << e.what() << "\n";
  }

  return 0;
}
