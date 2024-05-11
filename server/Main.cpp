#include"server.h"


using namespace boost::asio;

int main()
{ 
	io_context context;

	std::vector<boost::shared_ptr<server>> clients;

	boost::thread_group tg;

	tg.create_thread(boost::bind(accept_clients_thread, boost::ref(clients), boost::ref(context)));

	tg.create_thread(boost::bind(handle_clients_request, boost::ref(clients)));

	tg.join_all();

}


