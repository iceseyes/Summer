/*
 * plugin.h
 *
 * Basic structures for a new Summer Application
 *
 *  Created on: 28/ago/2014
 *      Author: Massimo Bianchi
 */

#ifndef SUMMER_PLUGIN_H_
#define SUMMER_PLUGIN_H_

#include <summer/views.h>
#include <summer/apps/ApplicationRegistry.h>
#include <summer/apps/Application.h>

#define SUMMER_APPLICATION(a) \
extern "C" void initApplication(const char *name) { \
	summer::ApplicationRegistry::instance().add<a>(name); \
}

#endif /* PLUGIN_H_ */
