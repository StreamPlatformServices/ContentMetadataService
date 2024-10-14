#pragma once
#include "StdAfx.hpp"
#include "Mappers/IHttpDataMapper.hpp"
#include "Routing/ICommandSelector.hpp"

namespace ContentMetadataApi
{
    class CONTENT_METADATA_API_EXPORT HttpServer 
    {
    public:
        explicit HttpServer(
            std::unique_ptr<Mappers::IHttpDataMapper> a_http_data_mapper,
            std::unique_ptr<Routing::ICommandSelector> a_command_selector);

        HttpServer(const HttpServer&) = delete;
        HttpServer(HttpServer&&) = delete;

        HttpServer& operator=(const HttpServer&) = delete;
        HttpServer& operator=(HttpServer&&) = delete;

        ~HttpServer();

        void startServer(uint16_t a_port);

        void stopServer();

    private:
        void startAccept();
        auto handleRequest(boost::asio::ip::tcp::socket a_socket) -> boost::asio::awaitable<void>;

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