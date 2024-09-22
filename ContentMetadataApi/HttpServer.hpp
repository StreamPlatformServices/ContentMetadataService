#pragma once
#include "StdAfx.hpp"
#include "Mappers/IHttpDataMapper.hpp"
#include "Routing/ICommandSelector.hpp"

namespace ContentMetadataApi
{
    class CONTENT_METADATA_API_EXPORT HttpServer 
    {
    public:
        HttpServer(
            std::unique_ptr<Mappers::IHttpDataMapper> a_http_data_mapper,
            std::unique_ptr<Routing::ICommandSelector> a_command_selector);

        ~HttpServer();

        void startServer(uint16_t port);

        void stopServer();

    private:
        void startAccept();
        boost::asio::awaitable<void> handleRequest(boost::asio::ip::tcp::socket socket);

    private:
        boost::asio::io_context m_ioc;
        boost::asio::ip::tcp::acceptor m_acceptor;
        std::thread m_server_thread;
        std::atomic<bool> m_running;

    private:
        std::unique_ptr<Mappers::IHttpDataMapper> m_http_data_mapper;
        std::unique_ptr<Routing::ICommandSelector> m_command_selector; 
    };
}