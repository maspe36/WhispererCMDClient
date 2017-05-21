#pragma once
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/enable_shared_from_this.hpp>

class Connection
	: public boost::enable_shared_from_this<Connection>
{
public:
	typedef boost::shared_ptr<Connection> pointer;

	static pointer Create(boost::asio::io_service& io_service, boost::asio::ip::tcp::resolver::iterator EndPointIterator);

	void Write(std::string data);
	void Close();
	void Connect();
private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::endpoint EndPoint;

	const std::string delimeter = "\n";
	boost::asio::streambuf sbuffer;

	Connection(boost::asio::io_service& io_service, boost::asio::ip::tcp::resolver::iterator EndPointIterator);

	void OnConnect(const boost::system::error_code& ErrorCode);
	void OnReceive(const boost::system::error_code& ErrorCode);
	void OnWrite(const boost::system::error_code&, size_t);
	void DoClose();
};
