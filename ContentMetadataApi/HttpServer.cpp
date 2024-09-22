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
        m_acceptor.async_accept([this](boost::beast::error_code ec, boost::asio::ip::tcp::socket socket)
            {
                if (!ec && m_running)
                {
                    boost::asio::co_spawn(m_ioc, handleRequest(std::move(socket)), boost::asio::detached);
                }

                if (m_running)
                {
                    startAccept();
                }
            });
    }

    boost::asio::awaitable<void> HttpServer::handleRequest(boost::asio::ip::tcp::socket a_socket)
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

    //C++ 17
    //void HttpServer::handleRequest(boost::asio::ip::tcp::socket a_socket)
    //{
    //    auto buffer = std::make_shared<boost::beast::flat_buffer>();
    //    auto request = std::make_shared<boost::beast::http::request<boost::beast::http::string_body>>();
    //    auto response = std::make_shared<boost::beast::http::response<boost::beast::http::string_body>>();

    //    // Asynchroniczny odczyt ¿¹dania HTTP
    //    boost::beast::http::async_read(a_socket, *buffer, *request,
    //        [this, socket = std::move(a_socket), buffer, request, response](boost::beast::error_code ec, std::size_t bytes_transferred) mutable
    //        {
    //            try {
    //                if (ec)
    //                {
    //                    std::cerr << "Read error: " << ec.message() << std::endl;
    //                    return;
    //                }

    //                auto http_request = m_http_data_mapper->httpRequestFrom(*request);
    //                auto command = m_command_selector->selectCommand(http_request);

    //                if (command)
    //                {
    //                    // Asynchroniczne wykonanie komendy
    //                    auto future_response = std::async(std::launch::async, [command]() { return command->execute(); });

    //                    // Obs³uga zakoñczenia wykonania zapytania
    //                    future_response.wait();
    //                    auto http_response = future_response.get();
    //                    *response = m_http_data_mapper->BoostBeastResponseFrom(http_response);
    //                }
    //                else
    //                {
    //                    response->set(boost::beast::http::field::content_type, "text/plain");
    //                    response->result(boost::beast::http::status::not_found);
    //                    response->body() = "Not Found";
    //                }

    //                // Asynchroniczny zapis odpowiedzi HTTP
    //                boost::beast::http::async_write(socket, *response,
    //                    [socket = std::move(socket)](boost::beast::error_code ec, std::size_t bytes_transferred) mutable
    //                    {
    //                        try {
    //                            if (ec)
    //                            {
    //                                std::cerr << "Write error: " << ec.message() << std::endl;
    //                                return;
    //                            }
    //                            // Zamkniêcie po³¹czenia
    //                            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    //                        }
    //                        catch (const std::exception& e) {
    //                            std::cerr << "Exception during write: " << e.what() << std::endl;
    //                        }
    //                    });
    //            }
    //            catch (const std::exception& e) {
    //                std::cerr << "Exception during read or processing: " << e.what() << std::endl;
    //            }
    //        });
    //}


    //void HttpServer::handleRequest(boost::asio::ip::tcp::socket a_socket) 
    //{
    //    auto buffer   = boost::beast::flat_buffer();
    //    auto request  = boost::beast::http::request<boost::beast::http::string_body>();
    //    auto response = boost::beast::http::response<boost::beast::http::string_body>();

    //    boost::beast::error_code ec;

    //    boost::beast::http::read(a_socket, buffer, request, ec);

    //    if (ec) 
    //    {
    //        if (ec == boost::beast::http::error::end_of_stream || ec == boost::asio::error::eof) 
    //        {

    //            std::cout << "End of stream detected." << std::endl;
    //        }
    //        else if (ec) 
    //        {
    //            std::cerr << "Error: " << ec.message() << std::endl;
    //        }
    //    }

    //    auto http_request = m_http_data_mapper->httpRequestFrom(request);

    //    auto command = m_command_selector->selectCommand(http_request); 
    //    
    //    if (command)
    //    {
    //        auto http_response = command->execute();
    //        response = m_http_data_mapper->BoostBeastResponseFrom(http_response);
    //    }
    //    else
    //    {
    //        response.set(boost::beast::http::field::content_type, "text/plain"); 
    //        response.result(boost::beast::http::status::not_found);
    //        response.body() = "Not Found";
    //    }
    //    
    //    boost::beast::http::write(a_socket, response);
    //    a_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);        
    //}
}