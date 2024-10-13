#pragma once
#include "IContentCommentRepository.hpp"
#include "../../../../ContentMetadataCommon/Threads/IThreadPoolManager.hpp"
#include "../../../../ContentMetadataCommon/DateTime/IDateTimeParser.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT ContentCommentRepository : public IContentCommentRepository
    {
    public:
        explicit ContentCommentRepository(
            std::shared_ptr<pqxx::work> a_transaction,
            std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager,
            std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser) noexcept;

        ContentCommentRepository(const ContentCommentRepository&) = delete;
        ContentCommentRepository(ContentCommentRepository&&) = delete;

        ContentCommentRepository& operator=(const ContentCommentRepository&) = delete;
        ContentCommentRepository& operator=(ContentCommentRepository&&) = delete;

        auto addAsync(ContentMetadataCore::Entities::ContentComment& a_content_comment) -> boost::asio::awaitable<boost::uuids::uuid> override;
        auto deleteAsync(boost::uuids::uuid a_content_comment_id) -> boost::asio::awaitable<void> override;
        auto getByContentIdAsync(boost::uuids::uuid a_content_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::ContentComment>> override;

    private:
        auto mapRowToComment(const pqxx::row& row) -> ContentMetadataCore::Entities::ContentComment;

    private:
        std::shared_ptr<pqxx::work> m_transaction;
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> m_thread_pool_manager;
        std::shared_ptr<ContentMetadataCommon::IDateTimeParser> m_date_time_parser;
    };
}
