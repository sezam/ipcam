#include "ipcam-socket-server.h"
#include "service.h"

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;
	vector<SocketServer*> servers;
	try
	{
		load_settings(argv[1], [&](string name, string url, int sock)
			{
				std::cout << "name=" << name << endl;
				std::cout << "url=" << url << endl;
				std::cout << "socket=" << sock << endl;
				servers.push_back(new SocketServer(io_service, url, name, sock));
			});

		io_service.run();
	}
	catch (const std::exception& e)
	{
		cout << "app error: " << e.what() << endl;
	}
	for (auto s : servers) delete s;
}
