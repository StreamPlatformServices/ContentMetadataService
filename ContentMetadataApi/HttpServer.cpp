#include "HttpServer.hpp"

namespace ContentMetadataApi
{
    HttpServer::HttpServer(
        std::unique_ptr<Mappers::IHttpDataMapper> a_http_data_mapper,
        std::unique_ptr<Routing::ICommandSelector> a_command_selector) 
        : m_ioc()
        , m_acceptor(m_ioc)
        , m_running(false)
        , m_http_data_mapper(std::move(a_http_data_mapper))
        , m_command_selector(std::move(a_command_selector))
    {   
    }

    HttpServer::~HttpServer() 
    {
        stopServer();
    }

    void HttpServer::startServer(uint16_t a_port)
    {
        m_running = true;
        boost::asio::ip::tcp::endpoint endpoint{ boost::asio::ip::tcp::v4(), a_port };
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(boost::asio::socket_base::reuse_address(true));
        m_acceptor.bind(endpoint);
        m_acceptor.listen();

        m_server_thread = std::thread([this]()
            {
                startAccept();
                m_ioc.run();
            });
    }

    void HttpServer::stopServer()
    {
        m_running = false;

        m_ioc.stop();

        if (m_server_thread.joinable())
        {
            m_server_thread.join();
        }
    }

    void HttpServer::startAccept()
    {
        m_acceptor.async_accept([this](boost::beast::error_code a_ec, boost::asio::ip::tcp::socket a_socket)
            {
                if (!a_ec && m_running)
                {
                    boost::asio::co_spawn(m_ioc, handleRequest(std::move(a_socket)), boost::asio::detached);
                }

                if (m_running)
                {
                    startAccept();
                }
            });
    }

    auto HttpServer::handleRequest(boost::asio::ip::tcp::socket a_socket) -> boost::asio::awaitable<void>
    {
        auto buffer = boost::beast::flat_buffer();
        auto request = boost::beast::http::request<boost::beast::http::string_body>();
        auto response = boost::beast::http::response<boost::beast::http::string_body>();

        try
        {
            co_await boost::beast::http::async_read(a_socket, buffer, request, boost::asio::use_awaitable);

            response.version(request.version());

            auto http_request = m_http_data_mapper->httpRequestFrom(request);

            auto command = m_command_selector->selectHttpCommand(http_request);

            if (command)
            {
                auto http_response = co_await command->executeAsync();
                response = m_http_data_mapper->BoostBeastResponseFrom(http_response);
            }
            else
            {
                response.set(boost::beast::http::field::content_type, "text/plain");
                response.result(boost::beast::http::status::not_found);
                response.body() = "Not Found";
            }

            
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "Request error: " << e.what() << std::endl;
            response.set(boost::beast::http::field::content_type, "text/plain");
            response.result(boost::beast::http::status::bad_request);
            response.body() = e.what();

        }
        catch (const std::exception& e)
        {
            std::cerr << "Request error: " << e.what() << std::endl;

            response.set(boost::beast::http::field::content_type, "text/plain");
            response.result(boost::beast::http::status::internal_server_error);
            response.body() = e.what();
        }
        

        co_await boost::beast::http::async_write(a_socket, response, boost::asio::use_awaitable);
        a_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
    }
}