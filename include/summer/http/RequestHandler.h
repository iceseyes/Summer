//
// RequestHandler.h
// ~~~~~~~~~~~~~~~~~~~
//

#ifndef SUMMER_REQUEST_HANDLER_HPP
#define SUMMER_REQUEST_HANDLER_HPP

#include <summer/logger.h>
#include <summer/http/Reply.h>
#include <summer/server/RootDispatcher.h>

#include <summer/net/URL.h>
#include <summer/apps/basic/ExceptionApplication.h>

#include <boost/noncopyable.hpp>

#include <string>


namespace summer { namespace http {

/// The common handler for all incoming requests.
template<
	class ConfigurationPolicy = server::conf::Configuration,
	class _Dispatcher = server::RootDispatcher<ConfigurationPolicy, Request>
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
			apps::basic::ExceptionApplication exApp;
			exApp.message = e.what();
			exApp(req, rep);
		}
	}


private:
	std::string _docRoot;	  /// documents directory.
};

typedef basic_requesthandler<> RequestHandler;

}}

#endif
