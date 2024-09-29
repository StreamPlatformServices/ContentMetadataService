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
            MOCK_METHOD(boost::asio::awaitable<ContentMetadataCore::Entities::Content>, getContentMetadataByIdAsync, (boost::uuids::uuid contentId), (override));
            MOCK_METHOD(boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>, getContentMetadataByOwnerIdAsync, (boost::uuids::uuid ownerId), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, editContentMetadataAsync, (boost::uuids::uuid contentId, ContentMetadataCore::Entities::Content content), (override));
            MOCK_METHOD(boost::asio::awaitable<boost::uuids::uuid>, addContentMetadataAsync, (ContentMetadataCore::Entities::Content content), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, deleteContentMetadataAsync, (boost::uuids::uuid contentId), (override));
        };
    }
}
