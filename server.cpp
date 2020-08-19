#include "server.h"

Session::Session(tcp::socket socket) : m_socket(std::move(socket))
{

}

void Session::start()
{
    start_read();
}

void Session::start_read()
{
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_data), [this, self](boost::system::error_code &ec, std::size_t length)
    { //ec без & ?
        if(!ec) {
            //Передача данных в обработчик бульки
            start_read();
        } // else ?
    });
}

Server::Server(io_service& ioservice, const tcp::endpoint& endpoint) : m_acceptor(ioservice, endpoint), m_socket(ioservice)
{
    start_accept();
};

void Server::start_accept()
{ //ec без & ?
    m_acceptor.async_accept(m_socket, [this](boost::system::error_code& ec) 
    {
        if(!ec)
        {
            //std::make_shared<Session>()
        }
        start_accept();  // Надо подумать как штатно завершить этот момент  
    });
}