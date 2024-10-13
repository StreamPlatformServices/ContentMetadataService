#include "LicenseRulesRepository_Should.hpp"

namespace ContentMetadataRepository_Tests
{
    TEST_F(LicenseRulesRepository_Should, AddLicenseRule)
    {
        ContentMetadataCore::Entities::LicenseRules license_rules;
        license_rules.m_uuid = m_license_rule_uuid;
        license_rules.m_price = 100;
        license_rules.m_type = ContentMetadataCore::Enums::LicenseType::Rent;
        license_rules.m_duration = ContentMetadataCore::Enums::LicenseDuration::OneDay;
        license_rules.m_content_id = m_content_id;

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->addAsync(license_rules),
            boost::asio::use_future
        );

        m_thread_pool.join();

        boost::uuids::uuid result = future.get();
        EXPECT_EQ(result, m_license_rule_uuid);

        m_transaction->commit();

        pqxx::work transaction(*m_connection);
        std::string sql = std::format(
            "SELECT * FROM {} WHERE {} = '{}'",
            ContentMetadataRepository::DatabaseKeys::LICENSE_RULES_TABLE_NAME,
            ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
            boost::uuids::to_string(m_license_rule_uuid)
        );
        auto res = transaction.exec(sql);
        EXPECT_EQ(res.size(), 1);
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME].as<std::string>(), boost::uuids::to_string(license_rules.m_uuid));
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::PRICE_COLUMN_NAME].as<int>(), license_rules.m_price);
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::LICENSE_TYPE_COLUMN_NAME].as<int>(), static_cast<int>(license_rules.m_type));
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::LICENSE_DURATION_COLUMN_NAME].as<int>(), static_cast<int>(*license_rules.m_duration));
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME].as<std::string>(), boost::uuids::to_string(license_rules.m_content_id));
    }

    TEST_F(LicenseRulesRepository_Should, GetLicenseRulesByContentId)
    {
        auto license_uuid = boost::uuids::random_generator()();
        insertLicenseRules(license_uuid, m_content_id);
        insertContent(boost::uuids::random_generator()());

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->getByContentIdAsync(m_content_id),
            boost::asio::use_future
        );

        auto rules = future.get();
        m_thread_pool.join();
        m_transaction->commit();

        EXPECT_EQ(rules.size(), 1);
        EXPECT_EQ(rules[0].m_uuid, license_uuid);
        EXPECT_EQ(rules[0].m_content_id, m_content_id);
    }

    TEST_F(LicenseRulesRepository_Should, DeleteLicenseRulesByContentId)
    {
        auto license_uuid = boost::uuids::random_generator()();
        insertLicenseRules(license_uuid, m_content_id);
        insertContent(boost::uuids::random_generator()());

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->deleteByContentIdAsync(m_content_id),
            boost::asio::use_future
        );

        m_thread_pool.join();
        future.get();
        m_transaction->commit();

        pqxx::work transaction_check(*m_connection);
        auto sql = std::format(
            "SELECT * FROM {} WHERE {} = '{}'",
            ContentMetadataRepository::DatabaseKeys::LICENSE_RULES_TABLE_NAME,
            ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
            boost::uuids::to_string(m_content_id)
        );
        auto res = transaction_check.exec(sql);
        EXPECT_EQ(res.size(), 0);

        transaction_check.abort();
    }
}
