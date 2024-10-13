#pragma once
#include "../../../StdAfx.hpp"
#include "../../../../ContentMetadataCore/Entities/ContentComment.hpp"

namespace ContentMetadataRepository
{
    class IContentCommentRepository
    {
    public:
        virtual ~IContentCommentRepository() = default;

        virtual auto addAsync(ContentMetadataCore::Entities::ContentComment& a_content_comment) -> boost::asio::awaitable<boost::uuids::uuid> = 0;
        virtual auto deleteAsync(boost::uuids::uuid a_content_comment_id) -> boost::asio::awaitable<void> = 0;
        virtual auto getByContentIdAsync(boost::uuids::uuid a_content_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::ContentComment>> = 0;
    };
}
