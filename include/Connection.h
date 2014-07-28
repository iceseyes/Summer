//
// Connection.h
// ~~~~~~~~~~~~~~
//

#ifndef SUMMER_CONNECTION_HPP
#define SUMMER_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <Reply.h>
#include <Request.h>
#include <RequestHandler.h>
#include <RequestParser.h>
#include <logger/logger.h>

namespace summer { namespace server { namespace http {

template<class RequestHandlerPolicy>
class Connection :
		public boost::enable_shared_from_this<Connection<RequestHandlerPolicy> >,
		private boost::noncopyable {
public:
	typedef RequestHandlerPolicy RequestHandler;
	typedef boost::enable_shared_from_this<Connection<RequestHandlerPolicy> > enabled_shared;
	using enabled_shared::shared_from_this;

	explicit Connection(boost::asio::io_service& io_service, RequestHandler& handler) :
			_strand(io_service), _socket(io_service), _requestHandler(handler) {}

	void start() {
		_socket.async_read_some(boost::asio::buffer(_buffer),
			_strand.wrap(
				boost::bind(&Connection::onRead, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
	}

	boost::asio::ip::tcp::socket& socket() { return _socket; }

private:
	void onRead(const boost::system::error_code& e,
			std::size_t bytes_transferred) {
		if (!e) {
			summer::logger::http.debugStream() <<
					"Connection::onRead() Parsing request " <<
					_buffer.data();

			boost::tribool result;
			boost::tie(result, boost::tuples::ignore) = _requestParser(_request,
					_buffer.data(), _buffer.data() + bytes_transferred);

			if (result) {
				summer::logger::http.debugStream() <<
						"Connection::onRead() Accepted buffer, creating reply";
				_requestHandler(_request, _reply);
				boost::asio::async_write(_socket, _reply.to_buffers(),
					_strand.wrap(
						boost::bind(&Connection::onWrite,
							shared_from_this(),
							boost::asio::placeholders::error)));
			} else if (!result) {
				summer::logger::http.warnStream() <<
						"Connection::onRead() BAD REQUEST, creating reply";

				_reply = http::Reply::stock_reply(http::Reply::bad_request);
				boost::asio::async_write(_socket, _reply.to_buffers(),
					_strand.wrap(
						boost::bind(&Connection::onWrite,
							shared_from_this(),
							boost::asio::placeholders::error)));
			} else {
				summer::logger::http.debugStream() <<
						"Connection::onRead() More data to read...";

				_socket.async_read_some(boost::asio::buffer(_buffer),
					_strand.wrap(
						boost::bind(&Connection::onRead,
							shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred)));
			}
		}
	}

	void onWrite(const boost::system::error_code& e) {
		// close conection.
		if (!e) {
			boost::system::error_code ignored_ec;
			_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		}
	}

	boost::asio::io_service::strand _strand; /// Strand to ensure the connection's handlers are not called concurrently.
	boost::asio::ip::tcp::socket _socket;

	RequestHandler& _requestHandler;
	boost::array<char, 8192> _buffer;
	Request _request;
	RequestParser _requestParser;

	Reply _reply;
};

}}}

#endif
