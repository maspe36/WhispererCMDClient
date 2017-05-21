#include <iostream>
#include <ostream>

#include <boost/thread.hpp>
#include "Connection.h"

int main()
{
	try {
		std::string Host = "127.0.0.1";
		std::string Port = "8888";

		boost::asio::io_service io_service;

		boost::asio::ip::tcp::resolver Resolver(io_service);

		boost::asio::ip::tcp::resolver::iterator EndPointIterator = Resolver.resolve(boost::asio::ip::tcp::resolver::query(Host, Port));

		boost::thread ClientThread(
			boost::bind(&boost::asio::io_service::run, &io_service));

		Connection::pointer connection = Connection::Create(io_service, EndPointIterator);

		connection->Connect();

		std::string data;
		while (data != "exit")
		{
			std::getline(std::cin, data);
			connection->Write(data);
		}

		connection->Close();
		ClientThread.join();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}