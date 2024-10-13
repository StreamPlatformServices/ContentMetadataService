#pragma once
#include "IContentRepository.hpp"
#include "../../../../ContentMetadataCommon/Threads/IThreadPoolManager.hpp"
#include "../../../../ContentMetadataCommon/DateTime/IDateTimeParser.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT ContentRepository : public IContentRepository
    {
    public:
        explicit ContentRepository(
            std::shared_ptr<pqxx::work> a_transaction,
            std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager,
            std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser) noexcept;

        ContentRepository(const ContentRepository&) = delete;
        ContentRepository(ContentRepository&&) = delete;

        ContentRepository& operator=(const ContentRepository&) = delete;
        ContentRepository& operator=(ContentRepository&&) = delete;

        auto getAllAsync(int a_limit, int a_offset) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> override;
        auto getByIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content> override;
        auto getByOwnerIdAsync(const boost::uuids::uuid& a_owner_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> override;
        auto addAsync(ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<boost::uuids::uuid> override;
        auto updateAsync(const ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<void> override;
        auto deleteAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<void> override;

    private:
        auto mapRowToContent(const pqxx::row& a_row) -> ContentMetadataCore::Entities::Content;

    private:
        //Note: Transactions are not shared between different threads.  
        //If one transaction is used by different instances of repositories it always appears synchronous. 
        //See DatabaseServices implementations.
        std::shared_ptr<pqxx::work> m_transaction;
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> m_thread_pool_manager;
        std::shared_ptr<ContentMetadataCommon::IDateTimeParser> m_date_time_parser;
    };
}
