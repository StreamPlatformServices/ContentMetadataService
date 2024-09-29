#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataApi/Mappers/EntityMappers/IContentDataMapper.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class ContentDataMapperMock : public ContentMetadataApi::Mappers::IContentDataMapper
        {
        public:
            ContentDataMapperMock() {
                std::cout << "MockContentDataMapper created!" << std::endl;
            }
            MOCK_METHOD(ContentMetadataCore::Entities::Content, contentEntityFrom, (const ContentMetadataApi::Dto::ContentDto& a_content_comment_dto), (const, override));
            MOCK_METHOD(ContentMetadataApi::Dto::ContentDto, contentDtoFrom, (const ContentMetadataCore::Entities::Content& a_content_comment_entity), (const, override));
        };
    }
}