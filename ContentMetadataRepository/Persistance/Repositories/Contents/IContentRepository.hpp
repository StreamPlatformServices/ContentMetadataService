#pragma once
#include "../../../StdAfx.hpp"
#include "../../../../ContentMetadataCore/Entities/Content.hpp"

namespace ContentMetadataRepository
{
    class IContentRepository
    {
    public:
        virtual ~IContentRepository() = default;

        virtual auto getAllAsync(int a_limit, int a_offset) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> = 0;
        virtual auto getByIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content> = 0;
        virtual auto getByOwnerIdAsync(const boost::uuids::uuid& a_owner_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> = 0;
        virtual auto addAsync(ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<boost::uuids::uuid> = 0;
        virtual auto updateAsync(const ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<void> = 0;
        virtual auto deleteAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<void> = 0;
    };
}
