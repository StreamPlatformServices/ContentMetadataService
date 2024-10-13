#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class ContentContractMock : public ContentMetadataCore::IContentsContract
        {
        public:
            MOCK_METHOD(boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>, getAllContentsAsync, (int limit, int offset), (override));
            MOCK_METHOD(boost::asio::awaitable<ContentMetadataCore::Entities::Content>, getContentMetadataByIdAsync, (const boost::uuids::uuid& content_id), (override));
            MOCK_METHOD(boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>, getContentMetadataByOwnerIdAsync, (const boost::uuids::uuid& owner_id), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, editContentMetadataAsync, (const boost::uuids::uuid& content_id, ContentMetadataCore::Entities::Content& content), (override));
            MOCK_METHOD(boost::asio::awaitable<boost::uuids::uuid>, addContentMetadataAsync, (ContentMetadataCore::Entities::Content& content), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, deleteContentMetadataAsync, (const boost::uuids::uuid& contentId), (override));
        };
    }
}
