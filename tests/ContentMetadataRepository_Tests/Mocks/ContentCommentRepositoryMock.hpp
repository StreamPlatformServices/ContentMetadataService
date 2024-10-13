#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataRepository/Persistance/Repositories/ContentComments/IContentCommentRepository.hpp"

namespace ContentMetadataRepository_Tests
{
    namespace Mocks
    {
        class ContentCommentRepositoryMock : public ContentMetadataRepository::IContentCommentRepository
        {
        public:
            MOCK_METHOD(boost::asio::awaitable<boost::uuids::uuid>, addAsync, (ContentMetadataCore::Entities::ContentComment& a_content_comment), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, deleteAsync, (boost::uuids::uuid a_content_comment_id), (override));
            MOCK_METHOD(boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::ContentComment>>, getByContentIdAsync, (boost::uuids::uuid a_content_id), (override));
        };
    }
}
