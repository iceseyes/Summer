/*
 * Summer - summer/http/Server.h
 * author: Massimo Bianchi 2014
 *
 * Define basic structures for a Summer HTTP Server.
 */

#ifndef SUMMER_HTTP_SERVER_H_
#define SUMMER_HTTP_SERVER_H_

#include <summer/conf/config.h>

#include <summer/http/basic.h>
#include <summer/http/RequestParser.h>
#include <summer/http/RootDispatcher.h>

#include <summer/server/Server.h>

namespace summer { namespace http {

/**
 * The basic Summer HTTP Server.
 *
 * A Summer HTTP Server is a Summer Server handling
 * Request to produce an Reply.
 * It uses [summer standard connection protocol](@ref summer::server::SimpleConnection)
 * that takes a one-shoot handler for every incoming request.
 */
template<typename configuration> using Server =
	summer::server::Server<configuration, Request, Reply>;

/**
 * The Standard Summer HTTP Server
 *
 * It is a an HTTP Server that using standard configuration method, defined in
 * summer::server::conf::Configuration
 */
using StdServer = Server<summer::server::conf::Configuration>;

}}

#endif /* SUMMER_HTTP_SERVER_H_ */
