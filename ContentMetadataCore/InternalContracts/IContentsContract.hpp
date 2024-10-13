#pragma once
#include "../Entities/Content.hpp"

namespace ContentMetadataCore
{
    class IContentsContract
    {
    public:
        virtual ~IContentsContract() = default;

        virtual boost::asio::awaitable<std::vector<Entities::Content>> getAllContentsAsync(int a_limit, int a_offset) = 0;
        virtual boost::asio::awaitable<Entities::Content> getContentMetadataByIdAsync(const boost::uuids::uuid& a_content_id) = 0;
        virtual boost::asio::awaitable<std::vector<Entities::Content>> getContentMetadataByOwnerIdAsync(const boost::uuids::uuid& a_owner_id) = 0;
        virtual boost::asio::awaitable<void> editContentMetadataAsync(const boost::uuids::uuid&, Entities::Content& a_content) = 0;
        virtual boost::asio::awaitable<boost::uuids::uuid> addContentMetadataAsync(Entities::Content& a_content) = 0;
        virtual boost::asio::awaitable<void> deleteContentMetadataAsync(const boost::uuids::uuid& a_content_id) = 0;
    };
}