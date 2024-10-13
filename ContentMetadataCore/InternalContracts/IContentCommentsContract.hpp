#pragma once
#include "../Entities/Content.hpp"

namespace ContentMetadataCore
{
    class IContentCommentsContract
    {
    public:
        virtual ~IContentCommentsContract() = default;

        virtual boost::asio::awaitable<boost::uuids::uuid> addContentCommentAsync(Entities::ContentComment& a_content_comment) = 0;
        virtual boost::asio::awaitable<void> deleteContentCommentAsync(const boost::uuids::uuid& a_content_comment_id) = 0;
    };
}