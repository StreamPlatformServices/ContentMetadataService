#pragma once
#include "InternalContracts/IContentsContract.hpp"

namespace ContentMetadataCore
{
    class CONTENT_METADATA_CORE_EXPORT ContentsService : public IContentsContract
    {
    public:

        boost::asio::awaitable<std::vector<Entities::Content>> getAllContentsAsync(int limit, int offset) override;
        boost::asio::awaitable<Entities::Content> getContentMetadataByIdAsync(boost::uuids::uuid contentId) override;
        boost::asio::awaitable<std::vector<Entities::Content>> getContentMetadataByOwnerIdAsync(boost::uuids::uuid ownerId) override;
        boost::asio::awaitable<void> editContentMetadataAsync(boost::uuids::uuid contentId, Entities::Content content) override;
        boost::asio::awaitable<boost::uuids::uuid> addContentMetadataAsync(Entities::Content content) override;
        boost::asio::awaitable<void> deleteContentMetadataAsync(boost::uuids::uuid contentId) override;
    };
}