//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2014 Massimo Bianchi
//

#include <summer/logger.h>
#include <summer/server/Server.h>
#include <summer/conf/config.h>
#include <summer/server/WebAppFolder.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace summer;

int main(int argc, char* argv[]) {
  try {
	  cout << "Starting summer server..." << endl;
	  logger::initLogger();
	  server::WebAppFolder::instance().scan();

	  server::StdServer s;
	  s.run();
  } catch (exception& e) {
	  logger::server.errorStream() << "exception: " << e.what();
  }

  return 0;
}
