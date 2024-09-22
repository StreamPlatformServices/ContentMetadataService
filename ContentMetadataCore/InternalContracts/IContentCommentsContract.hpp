#pragma once
#include "../Entities/Content.hpp"

namespace ContentMetadataCore
{
    
    //std::async with std::future creates new threads so better to use std::couritine for simple async I/O operations
    /*class CONTENT_METADATA_CORE_EXPORT IContentsContract
    {
    public:
        virtual ~IContentMetadataContract() = default;
        virtual std::future<std::vector<Content>> GetAllContentsAsync(int limit, int offset) = 0;
        virtual std::future<Content> GetContentMetadataByIdAsync(boost::uuids::uuid contentId) = 0;
        virtual std::future<std::vector<Content>> GetContentMetadataByOwnerIdAsync(boost::uuids::uuid ownerId) = 0;
        virtual std::future<void> EditContentMetadataAsync(boost::uuids::uuid contentId, Content content) = 0;
        virtual std::future<boost::uuids::uuid> AddContentMetadataAsync(Content content) = 0;
        virtual std::future<void> DeleteContentMetadataAsync(boost::uuids::uuid contentId) = 0;
    };*/

    class CONTENT_METADATA_CORE_EXPORT IContentCommentsContract
    {
    public:
        virtual ~IContentCommentsContract() = default;

        virtual boost::asio::awaitable<boost::uuids::uuid> addContentCommentAsync(Entities::ContentComment a_content_comment) = 0;
        virtual boost::asio::awaitable<void> deleteContentCommentAsync(boost::uuids::uuid a_content_comment_id) = 0;
    };
}