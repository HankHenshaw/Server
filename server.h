#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket);
    ~Session();
    void start();
    
private:
    void start_read();

    tcp::socket m_socket;
    std::array<char, 512> m_data;
};

class Server
{
public:
    Server(io_service& ioservice, const tcp::endpoint& endpoint);
private:
    void start_accept();

    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
};