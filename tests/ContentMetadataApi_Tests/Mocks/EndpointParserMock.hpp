#pragma once
#include <gmock/gmock.h>
#include "../../ContentMetadataApi/Parsers/IEndpointParser.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class EndpointParserMock : public ContentMetadataApi::Parsers::IEndpointParser
        {
        public:
            MOCK_METHOD(void, parseEndpoint, (const std::string& a_endpoint), (override));
            MOCK_METHOD((std::unordered_map<std::string, std::string>), getQueryParams, (), (const, noexcept, override));
            MOCK_METHOD(std::vector<std::string>, getRoutingPath, (), (const, noexcept, override));
        };
    }
}
