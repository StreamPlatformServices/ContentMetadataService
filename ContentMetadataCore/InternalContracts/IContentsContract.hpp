#pragma once
#include "../Entities/Content.hpp"

namespace ContentMetadataCore
{
    class IContentsContract
    {
    public:
        virtual ~IContentsContract() = default;

        virtual auto getAllContentsAsync(int a_limit, int a_offset) -> boost::asio::awaitable<std::vector<Entities::Content>> = 0;
        virtual auto getContentMetadataByIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<Entities::Content> = 0;
        virtual auto getContentMetadataByOwnerIdAsync(const boost::uuids::uuid& a_owner_id) -> boost::asio::awaitable<std::vector<Entities::Content>> = 0;
        virtual auto editContentMetadataAsync(const boost::uuids::uuid&, Entities::Content& a_content) -> boost::asio::awaitable<void> = 0;
        virtual auto addContentMetadataAsync(Entities::Content& a_content) -> boost::asio::awaitable<boost::uuids::uuid> = 0;
        virtual auto deleteContentMetadataAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<void> = 0;
    };
}