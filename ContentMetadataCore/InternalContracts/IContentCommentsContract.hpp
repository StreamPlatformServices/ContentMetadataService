#pragma once
#include "../Entities/Content.hpp"

namespace ContentMetadataCore
{
    class IContentCommentsContract
    {
    public:
        virtual ~IContentCommentsContract() = default;

        virtual auto addContentCommentAsync(Entities::ContentComment& a_content_comment) -> boost::asio::awaitable<boost::uuids::uuid> = 0;
        virtual auto deleteContentCommentAsync(const boost::uuids::uuid& a_content_comment_id) -> boost::asio::awaitable<void> = 0;
    };
}