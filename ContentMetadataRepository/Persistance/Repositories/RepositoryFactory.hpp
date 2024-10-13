#pragma once
#include "IRepositoryFactory.hpp"
#include "../../ContentMetadataCommon/Threads/IThreadPoolManager.hpp"
#include "../../ContentMetadataCommon/DateTime/IDateTimeParser.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT RepositoryFactory : public IRepositoryFactory
    {
    public:
        explicit RepositoryFactory(
            std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager,
            std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser) noexcept; 

        RepositoryFactory(const RepositoryFactory&) = delete;
        RepositoryFactory(RepositoryFactory&&) = delete;

        RepositoryFactory& operator=(const RepositoryFactory&) = delete;
        RepositoryFactory& operator=(RepositoryFactory&&) = delete;

        auto createContentCommentRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<IContentCommentRepository> override;
        auto createContentRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<IContentRepository> override;
        auto createLicenseRulesRepository(std::shared_ptr<pqxx::work> a_transaction) const-> std::unique_ptr<ILicenseRulesRepository> override;

    private:
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> m_thread_pool_manager;
        std::shared_ptr<ContentMetadataCommon::IDateTimeParser> m_date_time_parser;
    };

}
