#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include "server.h"

using namespace boost::asio;
using namespace boost::asio::ip;

int main(int argc, char* argv[])
{
    size_t bulkSize = 1;
    if(argc < 3 || argc > 3)
    {
        std::cerr << "Usage: bulkserver <port> <bulksize> \n";
        if(argc < 2)
        {
            return 1;
        }
        std::cerr << "bulksize was set to 1\n";
    } else {
        bulkSize = std::atoi(argv[2]);
    }

    io_service ioservice;

    tcp::endpoint ep(tcp::v4(), std::atoi(argv[1]));
    Server server(std::atoi(argv[1]), bulkSize);

    return 0;
}