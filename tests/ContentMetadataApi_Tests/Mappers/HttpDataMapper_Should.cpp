#pragma once
#include "HttpDataMapper_Should.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(HttpDataMapper_Should, MapHttpMethodCorrectly) {
        boost::beast::http::request<boost::beast::http::string_body> beast_request;

        beast_request.method(boost::beast::http::verb::get);
        EXPECT_EQ(m_sut->httpRequestFrom(beast_request).m_method, ContentMetadataApi::HttpMethod::Get);

        beast_request.method(boost::beast::http::verb::post);
        EXPECT_EQ(m_sut->httpRequestFrom(beast_request).m_method, ContentMetadataApi::HttpMethod::Post);

        beast_request.method(boost::beast::http::verb::put);
        EXPECT_EQ(m_sut->httpRequestFrom(beast_request).m_method, ContentMetadataApi::HttpMethod::Put);

        beast_request.method(boost::beast::http::verb::delete_);
        EXPECT_EQ(m_sut->httpRequestFrom(beast_request).m_method, ContentMetadataApi::HttpMethod::Delete);

        beast_request.method(boost::beast::http::verb::patch);
        EXPECT_EQ(m_sut->httpRequestFrom(beast_request).m_method, ContentMetadataApi::HttpMethod::Unknown);
    }

    TEST_F(HttpDataMapper_Should, MapHeadersCorrectly) {
        boost::beast::http::request<boost::beast::http::string_body> beast_request;
        beast_request.set(boost::beast::http::field::host, "localhost");
        beast_request.set(boost::beast::http::field::content_type, "application/json");

        auto http_request = m_sut->httpRequestFrom(beast_request);
        EXPECT_EQ(http_request.m_headers["Host"], "localhost");
        EXPECT_EQ(http_request.m_headers["Content-Type"], "application/json");
    }

    TEST_F(HttpDataMapper_Should, ConvertBoostBeastRequestToHttpRequest) {
        boost::beast::http::request<boost::beast::http::string_body> beast_request;
        beast_request.method(boost::beast::http::verb::post);
        beast_request.target("/api/v1/resource?query=test");
        beast_request.body() = "request body";
        beast_request.set(boost::beast::http::field::host, "localhost");

        EXPECT_CALL(*m_mock_endpoint_parser, parseEndpoint("/api/v1/resource?query=test")).Times(1);
        EXPECT_CALL(*m_mock_endpoint_parser, getQueryParams()).WillOnce(::testing::Return(std::unordered_map<std::string, std::string>{{"query", "test"}}));
        EXPECT_CALL(*m_mock_endpoint_parser, getRoutingPath()).WillOnce(::testing::Return(std::vector<std::string>{"api", "v1", "resource"}));

        auto http_request = m_sut->httpRequestFrom(beast_request);

        EXPECT_EQ(http_request.m_method, ContentMetadataApi::HttpMethod::Post);
        EXPECT_EQ(http_request.m_body, "request body");
        EXPECT_EQ(http_request.m_headers["Host"], "localhost");
        EXPECT_EQ(http_request.m_query_params["query"], "test");
        EXPECT_EQ(http_request.m_routing_path.size(), 3);
        EXPECT_EQ(http_request.m_routing_path[0], "api");
    }

    TEST_F(HttpDataMapper_Should, ConvertHttpResponseToBoostBeastResponse) {
        ContentMetadataApi::HttpResponse http_response;
        http_response.m_status_code = ContentMetadataApi::HttpStatusCode::Ok;
        http_response.m_body = "response body";
        http_response.m_headers["Content-Type"] = "application/json";

        auto beast_response = m_sut->BoostBeastResponseFrom(http_response);

        EXPECT_EQ(beast_response.result_int(), 200);
        EXPECT_EQ(beast_response.body(), "response body");
        EXPECT_EQ(beast_response["Content-Type"], "application/json");
    }

    TEST_F(HttpDataMapper_Should, MapInvalidHttpMethodCorrectly) {
        boost::beast::http::request<boost::beast::http::string_body> beast_request;

        beast_request.method(boost::beast::http::verb::trace);
        EXPECT_EQ(m_sut->httpRequestFrom(beast_request).m_method, ContentMetadataApi::HttpMethod::Unknown);
    }

    TEST_F(HttpDataMapper_Should, HandleMissingHeadersCorrectly) {
        boost::beast::http::request<boost::beast::http::string_body> beast_request;

        beast_request.method(boost::beast::http::verb::get);

        auto http_request = m_sut->httpRequestFrom(beast_request);

        EXPECT_EQ(http_request.m_headers.find("Host"), http_request.m_headers.end());
        EXPECT_EQ(http_request.m_headers.find("Content-Type"), http_request.m_headers.end());
    }

    TEST_F(HttpDataMapper_Should, HandleInvalidQueryParamsCorrectly) {
        boost::beast::http::request<boost::beast::http::string_body> beast_request;
        beast_request.target("/api/v1/resource?=test&flag&&param2");

        EXPECT_CALL(*m_mock_endpoint_parser, parseEndpoint("/api/v1/resource?=test&flag&&param2")).Times(1);
        EXPECT_CALL(*m_mock_endpoint_parser, getQueryParams())
            .WillOnce(::testing::Return(std::unordered_map<std::string, std::string>{{"", "test"}, { "flag", "" }, { "param2", "" }}));
        EXPECT_CALL(*m_mock_endpoint_parser, getRoutingPath())
            .WillOnce(::testing::Return(std::vector<std::string>{"api", "v1", "resource"}));

        auto http_request = m_sut->httpRequestFrom(beast_request);

        EXPECT_EQ(http_request.m_query_params[""], "test");
        EXPECT_EQ(http_request.m_query_params["flag"], "");
        EXPECT_EQ(http_request.m_query_params["param2"], "");
    }

    TEST_F(HttpDataMapper_Should, HandleEmptyRequestBodyCorrectly) {
        boost::beast::http::request<boost::beast::http::string_body> beast_request;
        beast_request.method(boost::beast::http::verb::get);
        beast_request.target("/api/v1/resource");

        beast_request.body() = "";

        EXPECT_CALL(*m_mock_endpoint_parser, parseEndpoint("/api/v1/resource")).Times(1);
        EXPECT_CALL(*m_mock_endpoint_parser, getQueryParams())
            .WillOnce(::testing::Return(std::unordered_map<std::string, std::string>{}));
        EXPECT_CALL(*m_mock_endpoint_parser, getRoutingPath())
            .WillOnce(::testing::Return(std::vector<std::string>{"api", "v1", "resource"}));

        auto http_request = m_sut->httpRequestFrom(beast_request);

        EXPECT_EQ(http_request.m_body, "");
    }


}