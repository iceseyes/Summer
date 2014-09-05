/*
 * summer - summer/server/SimpleConnection.h
 * author: Massimo Bianchi 2014
 *
 * Implements the basic protocol of a summer server
 */

#ifndef SUMMER_SIMPLECONNECTION_HPP
#define SUMMER_SIMPLECONNECTION_HPP

#include <summer/logger.h>

#include <summer/server/RootDispatcher.h>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/logic/tribool.hpp>

#include <memory>

namespace summer { namespace server {

/**
 * Implements the basic protocol of a summer server.
 *
 * SimpleConnection takes a client request, invoke the specific
 * RootDispatcher and produce a reply. At the end of this process
 * shutdown the connection.
 *
 * Requires three parameters (passed by server instance):
 * 1. _Conf: Configuration structure
 * 2. _Request: the specific request class
 * 3. _Reply: the class that handle the reply.
 */
template<typename _Conf, typename _Request, typename _Reply>
class SimpleConnection :
		public std::enable_shared_from_this<SimpleConnection<_Conf, _Request, _Reply>>,
		private boost::noncopyable {
public:
	using Configuration 	= _Conf;	//!< Configuration class used
	using Request 			= _Request; //!< Request class used
	using Reply 			= _Reply;	//!< Reply class user
	using RequestParser 	= typename Request::RequestParser;	//!< class to parse to convert net message in a Request
	using RootDispatcher 	= server::RootDispatcher<Configuration, Request, Reply>;	//!< class to select the handle for request

	/// Enable this class to optain a shared_ptr from this
	using enabled_shared 	= std::enable_shared_from_this<SimpleConnection<Configuration, Request, Reply>>;
	using enabled_shared::shared_from_this;

	/// Creates a new SimpleConnection using specific io_service and configuration.
	/// @param io_service [in] the network io service
	/// @param conf [in] configuration parameters.
	explicit SimpleConnection(boost::asio::io_service& io_service, const Configuration& conf) :
			_strand(io_service), _socket(io_service), _requestHandler(conf) {
	}

	/**
	 * Starting handling messages from socket.
	 *
	 * Main cicle of the protocol: when read any data from socket,
	 * process incoming message.
	 */
	void start() {
		that = shared_from_this();

		logger::http.debugStream()
			<< "SimpleConnection::start() "
			<< "Starting new connection.";

		_socket.async_read_some(
			boost::asio::buffer(_buffer),
			_strand.wrap(
				[&](const boost::system::error_code &e, std::size_t b){
					that->onRead(e, b);
				}));
	}

	/**
	 * Return socket used.
	 *
	 * @return socket currently used.
	 */
	boost::asio::ip::tcp::socket& socket() { return _socket; }

private:
	using ptr = std::shared_ptr<SimpleConnection<Configuration, Request, Reply>>;

	void onRead(const boost::system::error_code& e,
			std::size_t bytes_transferred) {
		if (!e) {
			logger::http.debugStream() <<
					"SimpleConnection::onRead() Parsing request " <<
					_buffer.data();

			boost::tribool result;
			boost::tie(result, boost::tuples::ignore) = _requestParser(_request,
					_buffer.data(), _buffer.data() + bytes_transferred);

			if (result) sendAnswer();
			else if (!result) sendBadRequest();
			else readMore();
		} else {
			logger::server.errorStream()
					<< "SimpleConnection::onRead(): "
					<< "An error occured while reading request: "
					<< e;
		}
	}

	void onWrite(const boost::system::error_code& e) {
		if (!e) { // close conection.
			boost::system::error_code ignored_ec;
			_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		} else {
			logger::server.errorStream()
					<< "SimpleConnection::onWrite(): "
					<< "An error occured while writing request: "
					<< e;
		}
	}

	void sendAnswer() {
		logger::http.debugStream() <<
				"SimpleConnection::sendAnswer() Accepted buffer, creating reply";

		_requestHandler(_request, _reply);
		send();
	}

	void sendBadRequest() {
		logger::http.warnStream() <<
				"SimpleConnection::sendBadRequest() BAD REQUEST, creating reply";

		_requestHandler.badRequest(_reply);
		send();

	}

	void readMore() {
		logger::http.debugStream() <<
				"SimpleConnection::readMore() More data to read...";

		_socket.async_read_some(
			boost::asio::buffer(_buffer),
			_strand.wrap(
				[&](const boost::system::error_code &e, std::size_t b){
					that->onRead(e, b);
				}));
	}

	void send() {
		boost::asio::async_write(_socket, _reply.to_buffers(),
			_strand.wrap(
				[&](const boost::system::error_code &e, std::size_t b){
					that->onWrite(e);
				}));
	}

	boost::asio::io_service::strand _strand; /// Strand to ensure the connection's handlers are not called concurrently.
	boost::asio::ip::tcp::socket _socket;

	ptr				that;
	Request 		_request;
	Reply 			_reply;
	RequestParser 	_requestParser;
	RootDispatcher 	_requestHandler;
	boost::array<char, 8192> _buffer;
};

}}

#endif
