#include "ipcam-socket-server.h"

void load_settings(char* path, function<void(string, string, int)> action)
{

    std::fstream ifs(path);
    ptree pt;

    read_ini(ifs, pt);

    for (auto& section : pt)
    {
        /*std::cout << '[' << section.first << "]" << endl;
        for (auto& key : section.second)
            std::cout << key.first << "=" << key.second.get_value<std::string>() << endl;
            */
        ptree sec = section.second;
        if (!sec.empty())
        {
            string host = sec.get("host", "localhost");
            string port = sec.get("port", "80");
            int sock = sec.get<int>("socket", 9090);
            string url = sec.get("url", "localhost");

            action(section.first, url, sock);
        }
    }
}
