#pragma once
#include "HttpDataMapper.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{        
        HttpDataMapper::HttpDataMapper(std::unique_ptr<Parsers::IEndpointParser> a_endpoint_parser) noexcept
            : m_endpoint_parser(std::move(a_endpoint_parser))
        {
        }

        HttpRequest HttpDataMapper::httpRequestFrom(const boost::beast::http::request<boost::beast::http::string_body>& a_request) const 
        {
            HttpRequest http_request;
            http_request.m_method = mapMethod(a_request.method());
            http_request.m_headers = mapHeaders(a_request);
            http_request.m_body = a_request.body();

            m_endpoint_parser->parseEndpoint(a_request.target());

            http_request.m_query_params = m_endpoint_parser->getQueryParams();
            http_request.m_routing_path = m_endpoint_parser->getRoutingPath();

            return http_request;
        }

        boost::beast::http::response<boost::beast::http::string_body> HttpDataMapper::BoostBeastResponseFrom(const HttpResponse& a_response) const
        {
            boost::beast::http::response<boost::beast::http::string_body> response;
            response.result(static_cast<boost::beast::http::status>(a_response.m_status_code));
            response.body() = a_response.m_body;

            for (const auto& header : a_response.m_headers) {
                response.set(header.first, header.second);
            }

            response.prepare_payload();
            return response;
        }

        HttpMethod HttpDataMapper::mapMethod(boost::beast::http::verb a_method) const
        {
            switch (a_method)
            {
            case boost::beast::http::verb::get:
                return HttpMethod::Get;
            case boost::beast::http::verb::post:
                return HttpMethod::Post;
            case boost::beast::http::verb::put:
                return HttpMethod::Put;
            case boost::beast::http::verb::delete_:
                return HttpMethod::Delete;
            default:
                return HttpMethod::Unknown;
            }
        }

        std::unordered_map<std::string, std::string> HttpDataMapper::mapHeaders(const boost::beast::http::request<boost::beast::http::string_body>& a_request) const
        {
            std::unordered_map<std::string, std::string> headers;
            for (const auto& field : a_request) 
            {
                auto header_field_sv = field.name_string();
                auto header_field = std::string(header_field_sv.data(), header_field_sv.size());

                auto header_value_sv = field.value();
                auto header_value = std::string(header_value_sv.data(), header_value_sv.size());

                headers[header_field] = header_value;
            }
            return headers;
        }
	}
}