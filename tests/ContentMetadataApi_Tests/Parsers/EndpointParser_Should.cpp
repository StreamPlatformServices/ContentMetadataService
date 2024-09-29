#pragma once
#include "EndpointParser_Should.hpp"

TEST_F(EndpointParser_Should, ParseRoutingPathCorrectly) 
{
    std::string endpoint = "/api/v1/resource/subresource";

    m_sut->parseEndpoint(endpoint);

    auto routing_path = m_sut->getRoutingPath();
    auto root_endpoint = m_sut->getRootEndpoint();

    ASSERT_EQ(routing_path.size(), 3);
    EXPECT_EQ(root_endpoint, "api");
    EXPECT_EQ(routing_path[0], "v1");
    EXPECT_EQ(routing_path[1], "resource");
    EXPECT_EQ(routing_path[2], "subresource");
}

TEST_F(EndpointParser_Should, ParseQueryParamsCorrectly) 
{
    std::string endpoint = "/api/resource?param1=value1&param2=value2";

    m_sut->parseEndpoint(endpoint);

    auto query_params = m_sut->getQueryParams();

    ASSERT_EQ(query_params.size(), 2);
    EXPECT_EQ(query_params["param1"], "value1");
    EXPECT_EQ(query_params["param2"], "value2");
}

TEST_F(EndpointParser_Should, HandleEmptyQueryParamValues) 
{
    std::string endpoint = "/api/resource?param1=value1&param2=&flag&param3=value3";

    m_sut->parseEndpoint(endpoint);

    auto query_params = m_sut->getQueryParams();

    ASSERT_EQ(query_params.size(), 4);
    EXPECT_EQ(query_params["param1"], "value1");
    EXPECT_EQ(query_params["param2"], "");
    EXPECT_EQ(query_params["flag"], "");
    EXPECT_EQ(query_params["param3"], "value3");
}


TEST_F(EndpointParser_Should, HandleMalformedQueryParams) 
{
    std::string endpoint = "/api/resource?param1=value1&param2=&flag&param3=value3&&s&s&&&=&";

    m_sut->parseEndpoint(endpoint);

    auto query_params = m_sut->getQueryParams();

    ASSERT_EQ(query_params.size(), 6);
    EXPECT_EQ(query_params["param1"], "value1");
    EXPECT_EQ(query_params["param2"], "");
    EXPECT_EQ(query_params["flag"], "");
    EXPECT_EQ(query_params["param3"], "value3");
    EXPECT_EQ(query_params["s"], "");
    EXPECT_EQ(query_params[""], "");
}


TEST_F(EndpointParser_Should, HandleEmptyPathCorrectly) 
{
    std::string endpoint = "/";

    m_sut->parseEndpoint(endpoint);

    auto routing_path = m_sut->getRoutingPath();

    ASSERT_EQ(routing_path.size(), 0);
}


TEST_F(EndpointParser_Should, HandleNoQueryParamsCorrectly) 
{
    std::string endpoint = "/api/resource";

    m_sut->parseEndpoint(endpoint);

    auto query_params = m_sut->getQueryParams();
    auto routing_path = m_sut->getRoutingPath();
    auto root_endpoint = m_sut->getRootEndpoint();

    ASSERT_EQ(query_params.size(), 0);
    ASSERT_EQ(routing_path.size(), 1);
    EXPECT_EQ(root_endpoint, "api");
    EXPECT_EQ(routing_path[0], "resource");
}

