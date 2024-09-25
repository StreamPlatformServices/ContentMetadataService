#pragma once
#include "../Entities/Content.hpp"

namespace ContentMetadataCore
{
    class CONTENT_METADATA_CORE_EXPORT IContentsContract
    {
    public:
        virtual ~IContentsContract() = default;

        virtual boost::asio::awaitable<std::vector<Entities::Content>> getAllContentsAsync(int limit, int offset) = 0;
        virtual boost::asio::awaitable<Entities::Content> getContentMetadataByIdAsync(boost::uuids::uuid contentId) = 0;
        virtual boost::asio::awaitable<std::vector<Entities::Content>> getContentMetadataByOwnerIdAsync(boost::uuids::uuid ownerId) = 0;
        virtual boost::asio::awaitable<void> editContentMetadataAsync(boost::uuids::uuid contentId, Entities::Content content) = 0;
        virtual boost::asio::awaitable<boost::uuids::uuid> addContentMetadataAsync(Entities::Content content) = 0;
        virtual boost::asio::awaitable<void> deleteContentMetadataAsync(boost::uuids::uuid contentId) = 0;
    };
}