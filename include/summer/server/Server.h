/*
 * Summer - summer/server/Server.h
 * author: Massimo Bianchi 2014
 *
 * The class representing a Generic Summer Server Instance
 */
#ifndef SUMMER_SERVER_HPP
#define SUMMER_SERVER_HPP

#include <summer/server/SimpleConnection.h>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <memory>
#include <functional>

/** \namespace summer
 * \brief summer namespace defines all structures of summer library.
 *
 * summer namespace is the basic namespace of all project Summer. It is composed by 5 main modules:
 * 1. summer::server: Defines a generic summer server.
 * 2. summer::http: Implements HTTP Protocol
 * 3. summer::net: Implements basic net utilies structures.
 * 4. summer::app: Define a Summer Application.
 * 5. summer::conf: Implements all configuration settings.
 */

/** \namespace summer::server
 * \brief Defines a generic summer server
 *
 * A Generic Summer Server is a server which handle some kind of a Request to produce a Reply using
 * a special protocol defined in an object called Connection. To achive this, summer server uses some
 * special classes called Application.
 *
 * An Application may be loaded dynamically (usually) or static (for some special application). Loading is demaned to
 * a special Class called summer::server::_WebAppFolder<>. This one populate a registry called summer::apps::ApplicationRegistry
 * which create on demanded the application instances.
 */

namespace summer { namespace server {

template<
	class ConfigurationPolicy,
	class _Request, class _Reply,
	template<class, class, class> class ConnectionPolicy>
class Server;

/// basic implementation functions
namespace __impl {
	/**
	 * Initialize Signals to catch
	 *
	 * Add all signals to catch in set passed.
	 * @param _stopSignals [out] set to fill with signals.
	 */
	void initStopSignals(boost::asio::signal_set &_stopSignals);

	/**
	 * Initialize Acceptor.
	 *
	 * Initialize socket acceptor with endpoint data.
	 * @param acceptor [out] The acceptor instance to initialize;
	 * @param endpoint [in] Socket data.
	 */
	void initAcceptor(
			boost::asio::ip::tcp::acceptor &acceptor,
			boost::asio::ip::tcp::endpoint endpoint);

	/**
	 * Prepare a TCP EndPoint.
	 *
	 * Prepare a TCP EndPoint from addess and port.
	 * @param ioService [in] isService to use as endpoint.
	 * @param address [in] server address to bind.
	 * @param port [in] server port to bind.
	 */
	boost::asio::ip::tcp::resolver::iterator getEndPoint(
		boost::asio::io_service &ioService,
		const std::string &address, const std::string &port);

	/**
	 * Run a server service.
	 *
	 * Creates _threadPool_size threads and bind them to service.
	 * @param ioService [in] Server service.
	 * @param _threadPool_size [in] Number of threads.
	 */
	void runService(std::function<void()> service, std::size_t _threadPool_size);
}

/**
 * A Summer Server represent a generic server instance.
 *
 * It takes four arguments
 * 1. ConfigurationPolicy: how to set up parameters of server.
 * 2. Request: the structure representing the request to server.
 * 3. Reply: the structure representing the answer of server.
 * 4. ConnectionPolicy: class representing the protocol. Default is a summer::server::SimpleConnection.
 */
template<
	class ConfigurationPolicy,
	class _Request, class _Reply,
	template<class, class, class> class ConnectionPolicy = SimpleConnection>
class Server : private boost::noncopyable {
public:
	using Configuration = ConfigurationPolicy;	//!< The Configuration Policy used
	using Request = _Request;					//!< The structure used to handle the request
	using Reply = _Reply;						//!< The structure used to produce the answer
	using Connection = ConnectionPolicy<Configuration, Request, Reply>;	//!< The protocol
	using ConnectionPtr = std::shared_ptr<Connection>;	//!< a Pointer to the class handing the protocol.

	explicit Server() :
			_threadPool_size(Configuration::instance().threadPoolSize()),
			_stopSignals(_ioService), _acceptor(_ioService),
			_currentConnection() {

		__impl::initStopSignals(_stopSignals);
		_stopSignals.async_wait(
				[&](const boost::system::error_code& error, int signal) {
					this->stop();
				});

		__impl::initAcceptor(_acceptor,
				*__impl::getEndPoint(
					_ioService,
					Configuration::instance().address(),
					Configuration::instance().port()));

		start();
	}

	/**
	 * Server main loop.
	 *
	 * The method create threads that waiting for a client connection.
	 * It represents the server main loop. When a client try to connect with
	 * this server, a thread is assigned as handle for that client requests.
	 */
	void run() {
		__impl::runService(
			[&](){ _ioService.run(); },
			_threadPool_size);
	}

private:
	void start() {
		_currentConnection.reset(new Connection(_ioService, Configuration::instance()));
		_acceptor.async_accept(
				_currentConnection->socket(),
				[&](const boost::system::error_code& e){
					this->accept(e);
				});
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
};

}}

#endif // SUMMER_SERVER_HPP
