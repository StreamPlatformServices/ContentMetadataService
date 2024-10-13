#pragma once
#include "ILicenseRulesRepository.hpp"
#include "../../../../ContentMetadataCommon/Threads/IThreadPoolManager.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT LicenseRulesRepository : public ILicenseRulesRepository
    {
    public:
        explicit LicenseRulesRepository(
            std::shared_ptr<pqxx::work> a_transaction,
            std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager) noexcept;

        LicenseRulesRepository(const LicenseRulesRepository&) = delete;
        LicenseRulesRepository(LicenseRulesRepository&&) = delete;

        LicenseRulesRepository& operator=(const LicenseRulesRepository&) = delete;
        LicenseRulesRepository& operator=(LicenseRulesRepository&&) = delete;

        auto addAsync(ContentMetadataCore::Entities::LicenseRules& a_license_rules) -> boost::asio::awaitable<boost::uuids::uuid> override;
        auto getByContentIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::LicenseRules>> override;
        auto updateAsync(const ContentMetadataCore::Entities::LicenseRules& a_license_rules) -> boost::asio::awaitable<void> override;
        auto deleteByContentIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<void> override;

    private:
        auto mapRowToLicenseRules(const pqxx::row& row) -> ContentMetadataCore::Entities::LicenseRules;

    private:
        std::shared_ptr<pqxx::work> m_transaction;
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> m_thread_pool_manager;
    };
}
