#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataApi/Mappers/EntityMappers/IContentCommentDataMapper.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class ContentCommentDataMapperMock : public ContentMetadataApi::Mappers::IContentCommentDataMapper
        {
        public:
            MOCK_METHOD(ContentMetadataCore::Entities::ContentComment, contentCommentEntityFrom, (const ContentMetadataApi::Dto::ContentCommentDto& a_content_comment_dto), (const, override));
            MOCK_METHOD(ContentMetadataApi::Dto::ContentCommentDto, contentCommentDtoFrom, (const ContentMetadataCore::Entities::ContentComment& a_content_comment_entity), (const, override));
        };
    }
}