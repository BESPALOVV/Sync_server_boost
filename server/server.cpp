#include"iostream"
#include"boost/asio.hpp"
#include"boost/bind.hpp"
#include"boost/thread/thread.hpp"
#include"boost/thread/recursive_mutex.hpp"
#include"boost/enable_shared_from_this.hpp"
#include "boost/noncopyable.hpp"
#include"server.h"


using namespace boost::asio;

boost::recursive_mutex mtx;

void accept_clients_thread(std::vector< boost::shared_ptr<server>>& clients, io_context& context)
{
	ip::tcp::acceptor acceptor(context, ip::tcp::endpoint(ip::tcp::v4(), 1313));
	for (;;)
	{
		std::cout << "wait connections" << std::endl;

		auto new_ = boost::shared_ptr<server>(new server(context,clients));
		acceptor.accept(new_->sock);
		mtx.lock();
		clients.push_back(new_);

		std::cout << "have +1 connection" << std::endl;
		mtx.unlock();
	}

}

void handle_clients_request(std::vector<boost::shared_ptr<server>>& clients)
{
	while (true)
	{
		mtx.lock();
		for (auto it = clients.begin(),e= clients.end();it != e;it++)
		{
			//std::cout << "handle colients" << std::endl;
			(*it)->answer_to_client();

			if ((*it)->destroy)
			{
				it =clients.erase(it);

				break;
			}
			//if (it == e) { break; }
		}
		mtx.unlock();
	}
}








void server::answer_to_client()
{
	try {
		//std::cout << "answer to client started" << std::endl;
		read_request();
		
		

		process_request();
	}
	catch (boost::system::error_code& ec)
	{
		stop();
	}
	/*if (timed_out())
	{
		stop();
	}*/
}

void server::read_request()
{
	//std::cout << "read started" << std::endl;
	if (sock.is_open()) {
		if (sock.available())
		{
			std::cout << "ALREADY READ::" << already_read << std::endl;
			
			already_read += sock.read_some(buffer(buff_read + already_read, 1024 - already_read));

			std::cout << "ALREADY READ::" << already_read << std::endl;
		}
	}
	else
	{
		std::cout << this->username << std::endl;
	
		std::cout << "no connection" <<  std::endl;

		stop();
	}
	
	//std::cout << "read stoped" << std::endl;
}

void server::stop()
{
	boost::system::error_code ec;

	//auto del= std::find(clients.begin(), clients.end(), this->shared_from_this());

	//clients.erase(del);

	sock.close(ec);
}

void server::process_request()
{
	//std::cout << "process request started" << std::endl;
	bool found = std::find(buff_read, buff_read + already_read, '\n') < buff_read + already_read;
	if (!found) { return; }

	auto last_ping = std::chrono::system_clock::now();

	size_t pos = std::find(buff_read, buff_read + already_read, '\n') - buff_read;

	std::string msg(buff_read, pos);

	std::cout << "THE MSG FROM CLIENT ::" << msg << std::endl;

	std::copy(buff_read + already_read, buff_read + 1024, buff_read);

	already_read -= pos + 1;

	if (msg.find("login") == 0) 
	{
		on_login(msg);

		this->last_ping = chrono::system_clock::now();
	}

	else if (msg.find("ping") == 0) 
	{
		timed_out();

		on_ping();

		this->last_ping = chrono::system_clock::now();
	}

	else if (msg.find("ask_clients") == 0) 
	{ 
		on_clients();
	}

	else
	{
		std::cerr << "invalid msg" << msg << std::endl;
		stop();
	}
}

void server::on_login(const std::string & msg)
{
	std::istringstream in(msg);

	in >> username >> username;

	sock.write_some( buffer("login_ok\n"));
}

void server::on_clients()
{
	std::string msg;
	//rec_mtx.lock();
	for (auto client : clients_)
	{
		msg += client->username + " ";
	}
	//rec_mtx.unlock();
	sock.write_some(buffer("clients " + msg + '\n'));
}

void server::on_ping()
{
	if (timed_out())
	{
		stop();
	}
	else {

		sock.write_some(clients_list_changed ? buffer("clients_listLchanged\n", 21) : buffer("ping ok\n", 8));

		clients_list_changed = false;
	}
}

bool server::timed_out()
{
	auto now = std::chrono::system_clock::now();

	std::chrono::duration<double> tim = now - last_ping;

	bool destroy = tim > std::chrono::seconds(5);

	std::cout << std::to_string(tim.count()) << std::endl;

	bool answer= tim > std::chrono::seconds(5);

	if (answer)
	{
		std::cout << "TIMED OUT FOR SOME CLIENT" << std::endl;

		this-> destroy = true;

		stop();
	}
	
	return answer;
	
	

}
