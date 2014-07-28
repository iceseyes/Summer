/**
 * Server.h
 *
 * author: Massimo Bianchi 2014
 */
#ifndef SUMMER_SERVER_HPP
#define SUMMER_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <Connection.h>
#include <RequestHandler.h>

namespace summer { namespace server {

namespace conf {
class Configuration;
}

/// The Server instance.
template<
	class ConfigurationPolicy = conf::Configuration,
	class RequestHandlerPolicy = http::RequestHandler,
	template<class> class ConnectionPolicy = http::Connection>
class Server : private boost::noncopyable {
public:
	typedef ConfigurationPolicy Configuration;
	typedef RequestHandlerPolicy RequestHandler;
	typedef ConnectionPolicy<RequestHandler> Connection;
	typedef boost::shared_ptr<Connection> ConnectionPtr;

	explicit Server() :
			_threadPool_size(Configuration::instance().threadPoolSize()),
			_stopSignals(_ioService), _acceptor(_ioService),
			_currentConnection(),
			_requestHandler(Configuration::instance()) {
		_stopSignals.add(SIGINT);
		_stopSignals.add(SIGTERM);
#if defined(SIGQUIT)
		_stopSignals.add(SIGQUIT);
#endif // defined(SIGQUIT)
		_stopSignals.async_wait(boost::bind(&Server::stop, this));

		// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		boost::asio::ip::tcp::resolver resolver(_ioService);
		boost::asio::ip::tcp::resolver::query query(
				Configuration::instance().address(),
				Configuration::instance().port());
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		_acceptor.open(endpoint.protocol());
		_acceptor.set_option(
				boost::asio::ip::tcp::acceptor::reuse_address(true));
		_acceptor.bind(endpoint);
		_acceptor.listen();

		start();

	}

	void run() {
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (std::size_t i = 0; i < _threadPool_size; ++i) {
			boost::shared_ptr<boost::thread> thread(
					new boost::thread(
						boost::bind(&boost::asio::io_service::run, &_ioService)));
			threads.push_back(thread);
		}

		for (std::size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
	}

private:
	void start() {
		_currentConnection.reset(new Connection(_ioService, _requestHandler));
		_acceptor.async_accept(_currentConnection->socket(),
				boost::bind(&Server::accept, this,
						boost::asio::placeholders::error));
	}
	void stop() { _ioService.stop(); }
	void accept(const boost::system::error_code& e) {
		if (!e) _currentConnection->start();
		start();
	}

	std::size_t _threadPool_size;				/// number of threads
	boost::asio::io_service _ioService;			/// I/O server service
	boost::asio::signal_set _stopSignals;		/// stop signals
	boost::asio::ip::tcp::acceptor _acceptor;	/// accept handler

	ConnectionPtr _currentConnection;	/// The next connection to be accepted.
	RequestHandler _requestHandler;		/// The handler for all incoming requests.
};

typedef Server<> StdServer;
}}

#endif // HTTP_SERVER3_SERVER_HPP
