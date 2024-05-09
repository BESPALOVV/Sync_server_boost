#include"server.h"


using namespace boost::asio;

int main()
{
	io_context context;

	boost::shared_ptr<server> first_client(new server(context));

	boost::thread_group tg;

	tg.create_thread(boost::bind(accept_clients_thread, boost::ref(first_client), boost::ref(context)));

	tg.create_thread(boost::bind(handle_clients_request, boost::ref(first_client)));

	tg.join_all();

}


