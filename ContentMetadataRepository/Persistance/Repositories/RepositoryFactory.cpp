#pragma once
#include "RepositoryFactory.hpp"
#include "ContentComments/ContentCommentRepository.hpp"
#include "Contents/ContentRepository.hpp"
#include "LicenseRules/LicenseRulesRepository.hpp"

namespace ContentMetadataRepository
{
    RepositoryFactory::RepositoryFactory(
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager,
        std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser) noexcept
        : m_thread_pool_manager(a_thread_pool_manager)
        , m_date_time_parser(a_date_time_parser) 
    {
    }

    auto RepositoryFactory::createContentCommentRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<IContentCommentRepository>
    {
        return std::make_unique<ContentCommentRepository>(a_transaction, m_thread_pool_manager, m_date_time_parser);
    }

    auto RepositoryFactory::createContentRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<IContentRepository>
    {
        return std::make_unique<ContentRepository>(a_transaction, m_thread_pool_manager, m_date_time_parser);
    }

    auto RepositoryFactory::createLicenseRulesRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<ILicenseRulesRepository>
    {
        return std::make_unique<LicenseRulesRepository>(a_transaction, m_thread_pool_manager);
    }
}
