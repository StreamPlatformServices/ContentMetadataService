#pragma once
#include <gtest/gtest.h>
#include "../../ContentMetadataApi/Mappers/IHttpDataMapper.hpp"
#include "../../ContentMetadataApi/Mappers/HttpDataMapper.hpp"
#include "../Mocks/EndpointParserMock.hpp" 


namespace ContentMetadataApi_Tests
{
    class HttpDataMapper_Should : public ::testing::Test 
    {

    protected:
        std::shared_ptr<Mocks::EndpointParserMock> m_mock_endpoint_parser;
        std::unique_ptr<ContentMetadataApi::Mappers::HttpDataMapper> m_sut;

        void SetUp() override 
        {
            m_mock_endpoint_parser = std::make_shared<Mocks::EndpointParserMock>();
            m_sut = std::make_unique<ContentMetadataApi::Mappers::HttpDataMapper>(m_mock_endpoint_parser);
        }
    };
}