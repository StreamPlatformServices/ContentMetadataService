#pragma once
#include "LicenseRulesRepository.hpp"
#include "../../../Constants/DatabaseKeys.hpp"

namespace ContentMetadataRepository
{
    LicenseRulesRepository::LicenseRulesRepository(
        std::shared_ptr<pqxx::work> a_transaction,
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager) noexcept
        : m_transaction(a_transaction)
        , m_thread_pool_manager(a_thread_pool_manager)
    {
    }

    auto LicenseRulesRepository::addAsync(ContentMetadataCore::Entities::LicenseRules& a_license_rules) -> boost::asio::awaitable<boost::uuids::uuid>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            if (a_license_rules.m_uuid.is_nil())
            {
                a_license_rules.m_uuid = boost::uuids::random_generator()();
            }

            std::string sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}) "
                "VALUES ('{}', {}, '{}', '{}', '{}');",
                DatabaseKeys::LICENSE_RULES_TABLE_NAME,
                DatabaseKeys::UUID_COLUMN_NAME,
                DatabaseKeys::PRICE_COLUMN_NAME,
                DatabaseKeys::LICENSE_TYPE_COLUMN_NAME,
                DatabaseKeys::LICENSE_DURATION_COLUMN_NAME,
                DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                boost::uuids::to_string(a_license_rules.m_uuid),
                a_license_rules.m_price,
                static_cast<int>(a_license_rules.m_type),
                a_license_rules.m_duration ? std::to_string(static_cast<int>(*a_license_rules.m_duration)) : "NULL",
                boost::uuids::to_string(a_license_rules.m_content_id)
            );

            m_transaction->exec0(sql);

            co_return a_license_rules.m_uuid;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error adding license rules: " << e.what() << std::endl;
            throw;
        }
    }

    auto LicenseRulesRepository::getByContentIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::LicenseRules>>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "SELECT * FROM {} WHERE {} = '{}';",
                DatabaseKeys::LICENSE_RULES_TABLE_NAME,
                DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                boost::uuids::to_string(a_content_id)
            );
            pqxx::result result = m_transaction->exec(sql);

            std::vector<ContentMetadataCore::Entities::LicenseRules> licenseRules;
            for (const auto& row : result)
            {
                licenseRules.push_back(mapRowToLicenseRules(row));
            }

            co_return licenseRules;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error fetching license rules by content id: " << e.what() << std::endl;
            throw;
        }
    }

    auto LicenseRulesRepository::updateAsync(const ContentMetadataCore::Entities::LicenseRules& a_license_rules) -> boost::asio::awaitable<void>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "UPDATE {} SET {} = {}, {} = '{}', {} = '{}' WHERE {} = '{}';",
                DatabaseKeys::LICENSE_RULES_TABLE_NAME,
                DatabaseKeys::PRICE_COLUMN_NAME,
                a_license_rules.m_price,
                DatabaseKeys::LICENSE_TYPE_COLUMN_NAME,
                static_cast<int>(a_license_rules.m_type),
                DatabaseKeys::LICENSE_DURATION_COLUMN_NAME,
                a_license_rules.m_duration ? std::to_string(static_cast<int>(*a_license_rules.m_duration)) : "NULL",
                DatabaseKeys::UUID_COLUMN_NAME,
                boost::uuids::to_string(a_license_rules.m_uuid)
            );

            m_transaction->exec0(sql);

            co_return;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error updating license rules: " << e.what() << std::endl;
            throw;
        }
    }

    auto LicenseRulesRepository::deleteByContentIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<void>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "DELETE FROM {} WHERE {} = '{}';",
                DatabaseKeys::LICENSE_RULES_TABLE_NAME,
                DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                boost::uuids::to_string(a_content_id)
            );

            m_transaction->exec0(sql);
            co_return;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error deleting license rules by content id: " << e.what() << std::endl;
            throw;
        }
    }

    auto LicenseRulesRepository::mapRowToLicenseRules(const pqxx::row& row) -> ContentMetadataCore::Entities::LicenseRules
    {
        ContentMetadataCore::Entities::LicenseRules license_rules;
        license_rules.m_uuid = boost::uuids::string_generator()(row[DatabaseKeys::UUID_COLUMN_NAME].as<std::string>());
        license_rules.m_price = row[DatabaseKeys::PRICE_COLUMN_NAME].as<int>();
        license_rules.m_type = static_cast<ContentMetadataCore::Enums::LicenseType>(row[DatabaseKeys::LICENSE_TYPE_COLUMN_NAME].as<int>());

        if (!row[DatabaseKeys::LICENSE_DURATION_COLUMN_NAME].is_null())
        {
            license_rules.m_duration = static_cast<ContentMetadataCore::Enums::LicenseDuration>(row[DatabaseKeys::LICENSE_DURATION_COLUMN_NAME].as<int>());
        }
        else
        {
            license_rules.m_duration = std::nullopt;
        }

        license_rules.m_content_id = boost::uuids::string_generator()(row[DatabaseKeys::CONTENT_UUID_COLUMN_NAME].as<std::string>());

        return license_rules;
    }
}
