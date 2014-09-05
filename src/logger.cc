/*
 * logger.cc
 *
 *  Created on: 02/mag/2014
 *      Author: massimo
 */

#include <summer/logger.h>

namespace summer { namespace logger {

LoggerCategory &root = log4cpp::Category::getRoot();
LoggerCategory &http = log4cpp::Category::getInstance(std::string("http"));
LoggerCategory &server = log4cpp::Category::getInstance(std::string("server"));
LoggerCategory &net = log4cpp::Category::getInstance(std::string("net"));
LoggerCategory &views = log4cpp::Category::getInstance(std::string("views"));

void initLogger() {
#ifdef LOGGER
	std::string initFileName = "log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);

#endif
}

}}



