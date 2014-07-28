/*
 * logger.cc
 *
 *  Created on: 02/mag/2014
 *      Author: massimo
 */

#include <logger/logger.h>

#ifdef LOGGER

log4cpp::Category &summer::logger::root = log4cpp::Category::getRoot();
log4cpp::Category &summer::logger::http = log4cpp::Category::getInstance(std::string("http"));

#else

EmptyLogger summer::logger::root;
EmptyLogger summer::logger::http;

#endif

void summer::logger::initLogger() {
#ifdef LOGGER
	std::string initFileName = "log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);

#endif
}



