#pragma once
#include "../Entities/Content.hpp"

namespace ContentMetadataCore
{
    class CONTENT_METADATA_CORE_EXPORT IContentCommentsContract
    {
    public:
        virtual ~IContentCommentsContract() = default;

        virtual boost::asio::awaitable<boost::uuids::uuid> addContentCommentAsync(Entities::ContentComment a_content_comment) = 0;
        virtual boost::asio::awaitable<void> deleteContentCommentAsync(boost::uuids::uuid a_content_comment_id) = 0;
    };
}