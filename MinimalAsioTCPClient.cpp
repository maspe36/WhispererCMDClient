#include <iostream>
#include <ostream>

#include <boost/thread.hpp>
#include "Connection.h"

//#include <boost/bind.hpp>
//#include <boost/asio.hpp>
//#include <boost/array.hpp>
//
//using namespace std;
//using namespace boost::asio::ip;
//
//class TCPClient
//{
//public:
//	TCPClient(boost::asio::io_service& IO_Service, tcp::resolver::iterator EndPointIter);
//
//	void Write();
//	void Close();
//private:
//	boost::asio::io_service& io_service;
//	tcp::socket socket;
//
//	static const size_t bufferLength = 128;
//	boost::array<char, bufferLength> buffer;
//
//private:
//	void OnConnect(const boost::system::error_code& ErrorCode,
//		tcp::resolver::iterator EndPointIter);
//
//	void OnReceive(const boost::system::error_code& ErrorCode);
//
//	void DoClose();
//};
//
//TCPClient::TCPClient(boost::asio::io_service& io_service, tcp::resolver::iterator EndPointIter)
//	: io_service(io_service), socket(io_service)
//{
//	tcp::endpoint EndPoint = *EndPointIter;
//
//	socket.async_connect(EndPoint,
//		boost::bind(&TCPClient::OnConnect, this, boost::asio::placeholders::error, ++EndPointIter));
//}
//
//void TCPClient::Close()
//{
//	io_service.post(
//		boost::bind(&TCPClient::DoClose, this));
//}
//void TCPClient::OnConnect(const boost::system::error_code& ErrorCode,
//	tcp::resolver::iterator EndPointIter)
//{
//	if (ErrorCode == 0)
//		// Successful connected
//	{
//		socket.async_receive(boost::asio::buffer(buffer.data(), bufferLength),
//			boost::bind(&TCPClient::OnReceive, this, boost::asio::placeholders::error));
//
//
//	}
//	else if (EndPointIter != tcp::resolver::iterator())
//	{
//		socket.close();
//		tcp::endpoint EndPoint = *EndPointIter;
//
//		socket.async_connect(EndPoint,
//			boost::bind(&TCPClient::OnConnect, this, boost::asio::placeholders::error, ++EndPointIter));
//	}
//}
//
//void TCPClient::OnReceive(const boost::system::error_code& ErrorCode)
//{
//	if (ErrorCode == 0)
//	{
//		std::cout << buffer.data() << std::endl;
//
//		socket.async_receive(boost::asio::buffer(buffer.data(), bufferLength),
//			boost::bind(&TCPClient::OnReceive, this, boost::asio::placeholders::error));
//	}
//	else {
//		DoClose();
//	}
//}
//
//void TCPClient::DoClose()
//{
//	socket.close();
//}

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
		//TCPClient Client(io_service, EndPointIterator);

		//std::cout << "Client started." << std::endl;

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