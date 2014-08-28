/*
 * Exceptions.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef SUMMER_SERVER_EXCEPTIONS_H_
#define SUMMER_SERVER_EXCEPTIONS_H_

#include <stdexcept>

namespace summer { namespace server { namespace exceptions {
struct BadRequestException : std::logic_error {
	BadRequestException(const std::string &uri) :
		std::logic_error("Bad Request \"" + uri + "\">") {};
};

struct ApplicationNotFoundException : std::logic_error {
	ApplicationNotFoundException(const std::string &name) :
		std::logic_error("Application named " + name + " not in registry.") {}
};

struct NoControllerDefined : std::logic_error {
	NoControllerDefined(const std::string &app, const std::string &resourcePath) :
		std::logic_error("No Controller defined for resource " + resourcePath + " in application \"" + app + "\"") {}
};

struct WebAppFolderDoesNotExist : std::logic_error {
	WebAppFolderDoesNotExist(const std::string &folder) :
		std::logic_error("Folder " + folder + " does not exist.") {}
};

struct ApplicationNotSupported : std::logic_error {
	ApplicationNotSupported(const std::string &folder) :
		std::logic_error("No Application found in " + folder) {}
};

}}}



#endif /* EXCEPTIONS_H_ */
