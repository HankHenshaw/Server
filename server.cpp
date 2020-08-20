#include "server.h"
#include <iostream>

Session::Session(tcp::socket socket, async::handle_t handle) : m_socket(std::move(socket)), m_handle(std::move(handle))
{

}

void Session::start()
{
    start_read();
}

void Session::start_read()
{
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_data), [this, self](boost::system::error_code ec, std::size_t length)
    {
        if(!ec) {
            //Передача данных в обработчик бульки
            async::receive(m_handle, m_data.data(), length);
            start_read();
        } else {
            async::disconnect(m_handle);
        }
    });
}

Session::~Session()
{
    async::disconnect(m_handle);
}

Server::Server(size_t port, size_t bulkSize) : m_acceptor(m_ioservice, tcp::endpoint(tcp::v4(), port)), m_socket(m_ioservice)
{
    //TODO:
    m_handle = async::connect(bulkSize);
    start_accept();
    m_ioservice.run();
}

void Server::start_accept()
{
    m_acceptor.async_accept(m_socket, [this](boost::system::error_code ec) 
    {
        if(!ec)
        {
            std::make_shared<Session>(std::move(m_socket), std::move(m_handle))->start();
        }
        start_accept();  // Надо подумать как штатно завершить этот момент  
    });
}