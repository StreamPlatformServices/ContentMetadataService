#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataRepository/Persistance/Repositories/Contents/IContentRepository.hpp"

namespace ContentMetadataRepository_Tests
{
    namespace Mocks
    {
        class ContentRepositoryMock : public ContentMetadataRepository::IContentRepository
        {
        public:
            MOCK_METHOD(boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>, getAllAsync, (int a_limit, int a_offset), (override));
            MOCK_METHOD(boost::asio::awaitable<ContentMetadataCore::Entities::Content>, getByIdAsync, (const boost::uuids::uuid& a_content_id), (override));
            MOCK_METHOD(boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>, getByOwnerIdAsync, (const boost::uuids::uuid& a_owner_id), (override));
            MOCK_METHOD(boost::asio::awaitable<boost::uuids::uuid>, addAsync, (ContentMetadataCore::Entities::Content& a_content), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, updateAsync, (const ContentMetadataCore::Entities::Content& a_content), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, deleteAsync, (const boost::uuids::uuid& a_content_id), (override));
        };
    }
}
