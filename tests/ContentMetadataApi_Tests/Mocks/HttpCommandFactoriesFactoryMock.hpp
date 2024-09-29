#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataApi/Routing/Factories/IHttpCommandFactoriesFactory.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class HttpCommandFactoriesFactoryMock : public ContentMetadataApi::Routing::IHttpCommandFactoriesFactory
        {
        public:
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Routing::IHttpCommandFactory>, createHttpCommandFactory, (const std::string&), (const, override));
        };
    }
}
