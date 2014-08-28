/*
 * logger.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#ifdef __WIN32
#	include <sys/types.h>
typedef _off64_t off64_t;
#endif

#ifdef LOGGER
#	include <log4cpp/Category.hh>
#	include <log4cpp/PropertyConfigurator.hh>
#endif

namespace summer { namespace logger {

#ifdef LOGGER
typedef log4cpp::Category LoggerCategory;

#else

class EmptyLogger {
	void debug(std::string &m) {}
	void info(std::string &m)  {}
	void warn(std::string &m)  {}
	void error(std::string &m) {}

	static EmptyLogger &getInstance(const std::string &name) { return summer::logger::root; }
};

typedef EmptyLogger LoggerCategory;

#endif

extern LoggerCategory &root;
extern LoggerCategory &http;
extern LoggerCategory &server;

void initLogger();

}}



#endif /* LOGGER_H_ */
