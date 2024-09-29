#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataApi/Routing/Factories/IHttpCommandFactory.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class HttpCommandFactoryMock : public ContentMetadataApi::Routing::IHttpCommandFactory
        {
        public:
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Routing::ICommand>, createGetCommand, (const ContentMetadataApi::HttpRequest&), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Routing::ICommand>, createPostCommand, (const ContentMetadataApi::HttpRequest&), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Routing::ICommand>, createPutCommand, (const ContentMetadataApi::HttpRequest&), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Routing::ICommand>, createDeleteCommand, (const ContentMetadataApi::HttpRequest&), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Routing::ICommand>, createPatchCommand, (const ContentMetadataApi::HttpRequest&), (const, override));
        };
    }
}
