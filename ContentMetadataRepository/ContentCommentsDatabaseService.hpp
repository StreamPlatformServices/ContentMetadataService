#pragma once
#include "../ContentMetadataCore/InternalContracts/IContentCommentsContract.hpp"
#include "Persistance/IUnitOfWork.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT ContentCommentsDatabaseService : public ContentMetadataCore::IContentCommentsContract
    {
    public:
        explicit ContentCommentsDatabaseService(std::shared_ptr<IUnitOfWork> a_unit_of_work) noexcept;

        ContentCommentsDatabaseService(const ContentCommentsDatabaseService&) = delete;
        ContentCommentsDatabaseService(ContentCommentsDatabaseService&&) = delete;

        ContentCommentsDatabaseService& operator=(const ContentCommentsDatabaseService&) = delete;
        ContentCommentsDatabaseService& operator=(ContentCommentsDatabaseService&&) = delete;

        auto addContentCommentAsync(ContentMetadataCore::Entities::ContentComment& a_content_comment) -> boost::asio::awaitable<boost::uuids::uuid> override;
        auto deleteContentCommentAsync(const boost::uuids::uuid& a_content_comment_id) -> boost::asio::awaitable<void> override;

    private:
        std::shared_ptr<IUnitOfWork> m_unit_of_work;
    };
}