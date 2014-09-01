/*
 * RootDispatcher.h
 *
 *  Created on: 25/giu/2014
 *      Author: massimo
 */

#ifndef ROOTDISPATCHER_H_
#define ROOTDISPATCHER_H_

namespace summer { namespace server {

/**
 * Select which Application can handle a client request to produce a reply.
 *
 * RootDispatcher depends by the protocol. The generic RootDispatcher comes with a
 * compile error (it is not defined). Clients must include specific protocol implementation
 * like http::RootDispatcher.
 *
 * Every RootDispatcher must implement an overload of operator apply:
 *
 * 		void operator()(const _Request &request, _Reply &reply)
 *
 * and badRequest() method that return a reply when request is not well formed:
 *
 * 		void badRequest(_Reply &reply)
 *
 * RootDispatcher must define types for RootDispatcher::Configuration, RootDispatcher::Request, RootDispatcher::Reply.
 */
template<class ConfigurationPolicy, class _Request, class _Reply>
class RootDispatcher;

}}



#endif /* ROOTDISPATCHER_H_ */
