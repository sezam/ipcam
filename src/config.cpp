#include "ipcam-socket-server.h"

void load_settings(char* path, function<void(string, string, int)> action)
{

	std::fstream ifs(path);
	ptree pt;

	read_ini(ifs, pt);

	if (pt.empty) cout << "ipcam: error read config file" << endl;

	for (auto& section : pt)
	{
		ptree sec = section.second;
		if (!sec.empty())
		{
			int sock = sec.get<int>("port", 9090);
			string url = sec.get("url", "localhost");

			action(section.first, url, sock);
		}
	}
}
