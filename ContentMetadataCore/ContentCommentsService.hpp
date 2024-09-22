#pragma once
#include "InternalContracts/IContentCommentsContract.hpp"

namespace ContentMetadataCore
{
    class CONTENT_METADATA_CORE_EXPORT ContentCommentsService : public IContentCommentsContract
    {
    public:
        boost::asio::awaitable<boost::uuids::uuid> addContentCommentAsync(Entities::ContentComment a_content_comment) override;
        boost::asio::awaitable<void> deleteContentCommentAsync(boost::uuids::uuid a_content_comment_id) override;
    };
}