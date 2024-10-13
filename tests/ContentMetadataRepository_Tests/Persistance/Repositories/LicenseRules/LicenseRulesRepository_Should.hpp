#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../../ContentMetadataRepository/Persistance/Repositories/LicenseRules/LicenseRulesRepository.hpp"
#include "../../../../../ContentMetadataRepository/Persistance/Repositories/LicenseRules/ILicenseRulesRepository.hpp"
#include "../../../Mocks/ThreadPoolManagerMock.hpp"
#include "../../../../../ContentMetadataRepository/Persistance/DatabaseSchema/DatabaseSchemaInitializer.hpp"
#include "../../../../../ContentMetadataCommon/Configuration/Config.hpp"
#include "../../../../../ContentMetadataRepository/Constants/DatabaseKeys.hpp"

namespace ContentMetadataRepository_Tests
{
    class LicenseRulesRepository_Should : public ::testing::Test
    {
    protected:
        boost::asio::thread_pool m_thread_pool{ 4 };
        std::shared_ptr<::testing::NiceMock<Mocks::ThreadPoolManagerMock>> m_thread_pool_manager_mock;
        std::shared_ptr<ContentMetadataCommon::Config> m_config;
        std::shared_ptr<pqxx::connection> m_connection;
        std::shared_ptr<pqxx::work> m_transaction;

        std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository> m_sut;
        boost::uuids::uuid m_content_id = boost::uuids::random_generator()();
        boost::uuids::uuid m_license_rule_uuid = boost::uuids::random_generator()();

        void SetUp() override
        {
            m_thread_pool_manager_mock = std::make_shared<::testing::NiceMock<Mocks::ThreadPoolManagerMock>>();

            m_config = std::make_shared<ContentMetadataCommon::Config>("test_config.ini", true);
            ContentMetadataRepository::DatabaseSchemaInitializer schema_initializer(m_config);
            schema_initializer.createContentMetadataTables();

            insertContent(m_content_id);

            std::string conn_str = std::format(
                "dbname={} user={} password={} host={} port={}",
                m_config->getDbName(),
                m_config->getDbUsername(),
                m_config->getDbPassword(),
                m_config->getDbHost(),
                m_config->getDbPort()
            );
            m_connection = std::make_shared<pqxx::connection>(conn_str);
            m_transaction = std::make_shared<pqxx::work>(*m_connection);

            ON_CALL(*m_thread_pool_manager_mock, getThreadPool())
                .WillByDefault(::testing::ReturnRef(m_thread_pool));

            m_sut = std::make_unique<ContentMetadataRepository::LicenseRulesRepository>(
                m_transaction,
                m_thread_pool_manager_mock);
        }

        void TearDown() override
        {
            std::string conn_str = std::format(
                "dbname={} user={} password={} host={} port={}",
                m_config->getDbName(),
                m_config->getDbUsername(),
                m_config->getDbPassword(),
                m_config->getDbHost(),
                m_config->getDbPort()
            );

            pqxx::connection connection(conn_str);
            pqxx::work transaction(connection);
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::LICENSE_RULES_TABLE_NAME) + " CASCADE;");
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_TABLE_NAME) + " CASCADE;");
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME) + " CASCADE;");
            transaction.commit();
        }

        void insertContent(boost::uuids::uuid a_content_id)
        {
            std::string conn_str = std::format(
                "dbname={} user={} password={} host={} port={}",
                m_config->getDbName(),
                m_config->getDbUsername(),
                m_config->getDbPassword(),
                m_config->getDbHost(),
                m_config->getDbPort()
            );

            pqxx::connection connection(conn_str);
            pqxx::work transaction(connection);
            std::string sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}, {}, {}, {}) "
                "VALUES ('{}', 'Sample Content', now(), 3600, 'Sample Description', '{}', '{}', '{}');",
                ContentMetadataRepository::DatabaseKeys::CONTENT_TABLE_NAME,
                ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::TITLE_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::UPLOAD_TIME_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::DURATION_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::DESCRIPTION_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::OWNER_ID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::VIDEO_FILE_ID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::IMAGE_FILE_ID_COLUMN_NAME,
                boost::uuids::to_string(a_content_id),
                boost::uuids::to_string(a_content_id),
                boost::uuids::to_string(a_content_id),
                boost::uuids::to_string(a_content_id)
            );
            transaction.exec0(sql);
            transaction.commit();
        }

        void insertLicenseRules(boost::uuids::uuid a_license_id, boost::uuids::uuid a_content_id)
        {
            std::string conn_str = std::format(
                "dbname={} user={} password={} host={} port={}",
                m_config->getDbName(),
                m_config->getDbUsername(),
                m_config->getDbPassword(),
                m_config->getDbHost(),
                m_config->getDbPort()
            );

            pqxx::connection connection(conn_str);
            pqxx::work transaction(connection);
            std::string sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}) "
                "VALUES ('{}', {}, {}, '{}', '{}');",
                ContentMetadataRepository::DatabaseKeys::LICENSE_RULES_TABLE_NAME,
                ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::PRICE_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::LICENSE_TYPE_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::LICENSE_DURATION_COLUMN_NAME,
                boost::uuids::to_string(a_license_id),
                100, 
                static_cast<int>(ContentMetadataCore::Enums::LicenseType::Rent), 
                boost::uuids::to_string(a_content_id),
                static_cast<int>(ContentMetadataCore::Enums::LicenseDuration::OneDay) 
            );
            transaction.exec0(sql);
            transaction.commit();
        }

    };
}
