#include "Connection.h"
#include <boost/bind/bind.hpp>
#include <iostream>

typedef boost::shared_ptr<Connection> pointer;

/* Given an io_service and a resolved EndPointIterator start doing an async_connect */
Connection::Connection(boost::asio::io_service& io_service, boost::asio::ip::tcp::resolver::iterator EndPointIter)
	: io_service(io_service), socket(io_service), EndPoint(*EndPointIter)
{
}

pointer Connection::Create(boost::asio::io_service & io_service, boost::asio::ip::tcp::resolver::iterator EndPointIterator)
{
	return pointer(new Connection(io_service, EndPointIterator));
}

void Connection::Write(std::string data)
{
	//Append delimeter to message
	data.append(delimeter);

	boost::asio::async_write(socket, boost::asio::buffer(data.c_str(), data.size()),
		boost::bind(&Connection::OnWrite, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

/* Method to be called after we write to the client */
void Connection::OnWrite(const boost::system::error_code &, size_t)
{
}

/* Closes the current connection cleanly */
void Connection::Close()
{
	io_service.post(
		boost::bind(&Connection::DoClose, shared_from_this()));
}

void Connection::Connect()
{
	socket.async_connect(EndPoint,
		boost::bind(&Connection::OnConnect, shared_from_this(), boost::asio::placeholders::error));
}

/* Method to be called on connection */
void Connection::OnConnect(const boost::system::error_code& ErrorCode)
{
	// Successful connected
	if (ErrorCode == nullptr)
	{
		/*socket.async_receive(boost::asio::buffer(buffer.data(), bufferLength),
			boost::bind(&Connection::OnReceive, shared_from_this(), boost::asio::placeholders::error));*/

		boost::asio::async_read_until(socket, sbuffer, delimeter, boost::bind(&Connection::OnReceive, shared_from_this(), boost::asio::placeholders::error));
	}
	else 
	{
		std::cout << "Connection to server failed!" << std::endl;
		std::cout << "IP Address: " << EndPoint.address() << std::endl;
		std::cout << "Port: " << EndPoint.port() << std::endl;
	}
}

/* Method to be called on a message recieved from the server */
void Connection::OnReceive(const boost::system::error_code& ErrorCode)
{
	if (ErrorCode == nullptr)
	{
		//// Get the message from the client as a string from the char array
		//std::string data(buffer.begin(), buffer.end());
		//data.erase(std::remove(data.begin(), data.end(), '\0'), data.end());

		//std::cout << data << std::endl;

		//// After we print the message, empty the buffer
		//buffer.assign(0);

		//socket.async_receive(boost::asio::buffer(buffer.data(), bufferLength),
		//	boost::bind(&Connection::OnReceive, shared_from_this(), boost::asio::placeholders::error));

		// Get the string from the client
		boost::asio::streambuf::const_buffers_type bufs = sbuffer.data();
		std::string data(
			boost::asio::buffers_begin(bufs),
			boost::asio::buffers_begin(bufs) + sbuffer.size());

		std::string message(data.substr(0, data.size() - 1));

		// Send the string minus the delimiter
		std::cout << "Server: " << message << std::endl;

		// Empty the stream buffer(?)
		sbuffer.consume(sbuffer.size());

		boost::asio::async_read_until(socket, sbuffer, delimeter, boost::bind(&Connection::OnReceive, shared_from_this(), boost::asio::placeholders::error));
	}
	else {
		DoClose();
	}
}

/* Method to be called when the connection closes */
void Connection::DoClose()
{
	std::cout << "Lost connection to server!" << std::endl;
	socket.close();
}
