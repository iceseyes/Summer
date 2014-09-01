/*
 * Summer - summer/http/RootDispatcher.h
 * Author: massimo bianchi 2014
 *
 * Implements RootDispatcher for HTTP Server
 */

#ifndef SUMMER_HTTP_ROOTDISPATCHER_H_
#define SUMMER_HTTP_ROOTDISPATCHER_H_

#include <summer/Exceptions.h>
#include <summer/conf/config.h>

#include <summer/server/RootDispatcher.h>

#include <summer/apps/ApplicationRegistry.h>
#include <summer/apps/basic/FileSystemApplication.h>
#include <summer/apps/basic/ExceptionApplication.h>

namespace summer {

class Application;

namespace http {

struct Request;
struct Reply;

/**
 * Implements summer::server::RootDispatcher for HTTP Protocol.
 *
 * Handles a Request in a Reply through operator apply and create a badRequest
 * using ad hoc method.
 */
class RootDispatcher {
public:
	RootDispatcher() {}
	virtual ~RootDispatcher() {}

	/**
	 * Convert a Request to a Reply.
	 *
	 * If request can not be handle return a Application which search
	 * a file with request URI Name. If any exception it is thrown, send an
	 * Exception Thrown page as reply.
	 *
	 * @param request [in] a Request to handle.
	 * @param reply [out] a Reply to send to client.
	 */
	virtual void operator()(const Request &request, Reply &reply);

	/**
	 * Creates a bad request reply.
	 *
	 * @param reply [out] the object to set as Bad Request.
	 */
	virtual void badRequest(Reply &reply);

protected:
	/**
	 * Return an Application that search request on file system.
	 *
	 * @return the Application for file system searching.
	 */
	virtual Application &filesystemApplication() = 0;

	/**
	 * Return an Application to handle Exceptions.
	 *
	 * @return the Application to handle exceptions.
	 */
	virtual Application &exceptionApplication(const std::string &message) = 0;

private:
	Application &select(const Request &request);
};

}

namespace server {

/**
 * Defines summer::server::RootDispatcher for HTTP Server. @see(RootDispatcher)
 *
 * RootDispatcher for http::Server must takes as _Request and http::Request, to produce a http::Reply.
 * Configuration must by passed as template argument.
 *
 * Under the cover this class uses summer::http::RootDispatcher.
 */
template<class ConfigurationPolicy>
class RootDispatcher<ConfigurationPolicy, http::Request, http::Reply> : public http::RootDispatcher {
public:
	using Configuration = ConfigurationPolicy;	//!< Configuration class used
	using Request 		= http::Request;		//!< Request class used (http implementation)
	using Reply 		= http::Reply;			//!< Reply class used (http implementation)

	/// Creates a new RootDispatcher using config as configuration
	/// @param config [in] configuration to use.
	RootDispatcher(const Configuration &config) : fsApp(config) {}
	virtual ~RootDispatcher() {}

protected:
	virtual Application &filesystemApplication() { return fsApp; }
	virtual Application &exceptionApplication(const std::string &message) {
		exApp.message = message;
		return exApp;
	}

private:
	using FileSystemApplication = apps::basic::FileSystemApplication<Configuration> ;
	using ExceptionApplication 	= apps::basic::ExceptionApplication;

	FileSystemApplication fsApp; /// the last
	ExceptionApplication exApp;
};

}}


#endif /* HTTP_ROOTDISPATCHER_H_ */
