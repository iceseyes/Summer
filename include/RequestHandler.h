//
// RequestHandler.h
// ~~~~~~~~~~~~~~~~~~~
//

#ifndef SUMMER_REQUEST_HANDLER_HPP
#define SUMMER_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>
#include <RootDispatcher.h>
#include <Reply.h>
#include <logger/logger.h>
#include <net/URL.h>
#include <basic_apps/ExceptionApplication.h>

namespace summer { namespace server { namespace http {

/// The common handler for all incoming requests.
template<
	class ConfigurationPolicy = conf::Configuration,
	class _Dispatcher = RootDispatcher<ConfigurationPolicy>
> class basic_requesthandler :
		public _Dispatcher,
		private boost::noncopyable {
public:
	typedef net::URL URL;
	typedef ConfigurationPolicy Configuration;
	typedef _Dispatcher	Dispatcher;
	using Dispatcher::select;

	explicit basic_requesthandler(const Configuration& config) :
			Dispatcher(config), _docRoot(config.docRoot()) {}

	void operator()(const Request& req, Reply& rep) {
		try{
			select(req)(req, rep);
		} catch (std::exception &e) {
			logger::http.errorStream() << "RequestHandler() Error: " << e.what();
			basic_apps::ExceptionApplication exApp;
			exApp.message = e.what();
			exApp(req, rep);
		}
	}


private:
	std::string _docRoot;	  /// documents directory.
};

typedef basic_requesthandler<> RequestHandler;

}}}

#endif
