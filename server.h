#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include "async.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket, async::handle_t handle);
    ~Session();
    void start();
    
private:
    void start_read();
    
    tcp::socket m_socket;
    async::handle_t m_handle;
    std::array<char, 512> m_data;
};

class Server
{
public:
    Server(size_t port, size_t bulkSize);
private:
    void start_accept();

    async::handle_t m_handle;
    boost::asio::io_service m_ioservice;
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
};