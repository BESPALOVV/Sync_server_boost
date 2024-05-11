// server.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once
#ifndef SERVER_H
#define SERVER_H
#include"iostream"
#include"boost/asio.hpp"
#include"boost/bind.hpp"
#include"boost/thread/thread.hpp"
#include"boost/thread/recursive_mutex.hpp"
#include"boost/enable_shared_from_this.hpp"
#include "boost/noncopyable.hpp"
#include"server.h"

using namespace boost::asio;

class server : public boost::enable_shared_from_this<server>, boost::noncopyable
{
public:
	server(io_context& context, std::vector<boost::shared_ptr<server>>& clients) :sock(context),clients_(clients), clients_list_changed(false), already_read(0) {}

	void answer_to_client();

private:

	void read_request();

	void process_request();

	void stop();

	void on_login(const std::string& msg);

	void on_ping();

	void on_clients();

	bool timed_out();



	ip::tcp::socket sock;

	char buff_read[1024];

	size_t already_read;

	std::string username;

	bool clients_list_changed;

	std::chrono::time_point<std::chrono::system_clock> last_ping;

	bool destroy = false;

    std::vector<boost::shared_ptr<server>>& clients_;

	friend	void accept_clients_thread(std::vector< boost::shared_ptr<server>>& clients, io_context& context);

	friend void handle_clients_request(std::vector<boost::shared_ptr<server>>& clients);
};




#endif // !SERVER_H



// TODO: установите здесь ссылки на дополнительные заголовки, требующиеся для программы.