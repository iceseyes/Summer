/*
 * Summer - server/Server.cc
 * author: Massimo Bianchi 2014
 *
 * Implements basic network function
 */

#include <summer/server/Server.h>

#include <vector>
#include <thread>

using namespace std;
using namespace boost::asio;
using boost::bind;

using acceptor = ip::tcp::acceptor;
using endpoint = ip::tcp::endpoint;
using resolver = ip::tcp::resolver;

using Thread		= thread;
using ThreadPtr 	= shared_ptr<Thread>;
using ThreadList 	= vector<ThreadPtr>;

namespace summer { namespace server { namespace __impl {

void initStopSignals(signal_set &_stopSignals) {
	_stopSignals.add(SIGINT);
	_stopSignals.add(SIGTERM);
#if defined(SIGQUIT)
	_stopSignals.add(SIGQUIT);
#endif // defined(SIGQUIT)
}

void initAcceptor(acceptor &acceptor, endpoint endpoint) {
	acceptor.open(endpoint.protocol());
	acceptor.set_option(
			boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor.bind(endpoint);
	acceptor.listen();
}

resolver::iterator getEndPoint(io_service &ioService,
		const string &address, const string &port) {
	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	resolver resolver(ioService);
	resolver::query query(address, port);
	return resolver.resolve(query);
}

void runService(boost::asio::io_service *ioService, std::size_t _threadPool_size) {
	ThreadList threads;
	for (std::size_t i = 0; i < _threadPool_size; ++i) {
		ThreadPtr thread(
			new Thread(bind(&io_service::run, ioService)));
		threads.push_back(thread);
	}

	for (std::size_t i = 0; i < threads.size(); ++i) threads[i]->join();
}

}}}

