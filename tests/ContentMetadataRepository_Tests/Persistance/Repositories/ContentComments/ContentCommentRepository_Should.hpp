#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../../ContentMetadataRepository/Persistance/Repositories/ContentComments/ContentCommentRepository.hpp"
#include "../../../../../ContentMetadataRepository/Persistance/Repositories/ContentComments/IContentCommentRepository.hpp"
#include "../../../Mocks/ThreadPoolManagerMock.hpp"
#include "CommonMocks/DateTimeParserMock.hpp"
#include "../../../../../ContentMetadataRepository/Persistance/DatabaseSchema/DatabaseSchemaInitializer.hpp"
#include "../../../../../ContentMetadataCommon/Configuration/Config.hpp"
#include "../../../../../ContentMetadataRepository/Constants/DatabaseKeys.hpp"

namespace ContentMetadataRepository_Tests
{
    class ContentCommentRepository_Should : public ::testing::Test
    {
    protected:
        boost::asio::thread_pool m_thread_pool{ 4 };
        std::shared_ptr<::testing::NiceMock<Mocks::ThreadPoolManagerMock>> m_thread_pool_manager_mock;
        std::shared_ptr<::testing::NiceMock<::Mocks::DateTimeParserMock>> m_date_time_parser_mock;
        std::shared_ptr<ContentMetadataCommon::Config> m_config;
        std::shared_ptr<pqxx::connection> m_connection;
        std::shared_ptr<pqxx::work> m_transaction;

        std::unique_ptr<ContentMetadataRepository::IContentCommentRepository> m_sut;

        boost::uuids::uuid m_content_id = boost::uuids::random_generator()();
        boost::uuids::uuid m_content_id_2 = boost::uuids::random_generator()();

        void SetUp() override
        {
            m_thread_pool_manager_mock = std::make_shared<::testing::NiceMock<Mocks::ThreadPoolManagerMock>>();
            m_date_time_parser_mock    = std::make_shared<::testing::NiceMock<::Mocks::DateTimeParserMock>>();

            m_config = std::make_shared<ContentMetadataCommon::Config>("config.ini");
            ContentMetadataRepository::DatabaseSchemaInitializer schema_initializer(m_config);
            schema_initializer.createContentMetadataTables();

            insertContent(m_content_id);
            insertContent(m_content_id_2);

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

            m_sut = std::make_unique<ContentMetadataRepository::ContentCommentRepository>(
                m_transaction,
                m_thread_pool_manager_mock, 
                m_date_time_parser_mock);
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
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME) + " CASCADE;");
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_TABLE_NAME) + " CASCADE;");
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::LICENSE_RULES_TABLE_NAME) + " CASCADE;");
            transaction.commit();
        }


        std::string formatPgSqlTimestamp(std::chrono::system_clock::time_point tp)
        {
            std::time_t time = std::chrono::system_clock::to_time_t(tp);
            std::tm tm = *std::gmtime(&time);

            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }

        void insertContent(boost::uuids::uuid a_uuid)
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
                boost::uuids::to_string(a_uuid),
                boost::uuids::to_string(a_uuid),
                boost::uuids::to_string(a_uuid),
                boost::uuids::to_string(a_uuid)
            );
            transaction.exec0(sql);
            transaction.commit();
        }

        void insertComment(boost::uuids::uuid a_comment_id)
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
                "INSERT INTO {} ({}, {}, {}, {}, {}) VALUES ('{}', '{}', now(), '{}', '{}')",
                ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::BODY_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CREATION_TIME_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::USER_NAME_COLUMN_NAME,
                boost::uuids::to_string(a_comment_id),
                "Sample Comment",
                boost::uuids::to_string(m_content_id),
                "test_user"
            );
            transaction.exec0(sql);
            transaction.commit();
        }

        void insertComments(std::chrono::system_clock::time_point a_time_point, const std::vector<boost::uuids::uuid>& a_comments_ids)
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
                "INSERT INTO {} ({}, {}, {}, {}, {}) VALUES ('{}', '{}', '{}', '{}', '{}')",
                ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::BODY_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CREATION_TIME_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::USER_NAME_COLUMN_NAME,
                boost::uuids::to_string(a_comments_ids[0]),
                "Sample Comment 1",
                formatPgSqlTimestamp(a_time_point),
                boost::uuids::to_string(m_content_id),
                "test_user1"
            );
            transaction.exec0(sql);

            sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}) VALUES ('{}', '{}', '{}', '{}', '{}')",
                ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::BODY_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CREATION_TIME_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::USER_NAME_COLUMN_NAME,
                boost::uuids::to_string(a_comments_ids[1]),
                "Sample Comment 2",
                formatPgSqlTimestamp(a_time_point),
                boost::uuids::to_string(m_content_id),
                "test_user2"
            );
            transaction.exec0(sql);

            sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}) VALUES ('{}', '{}', '{}', '{}', '{}')",
                ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::BODY_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CREATION_TIME_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::USER_NAME_COLUMN_NAME,
                boost::uuids::to_string(a_comments_ids[2]),
                "Sample Comment 3",
                formatPgSqlTimestamp(a_time_point),
                boost::uuids::to_string(m_content_id),
                "test_user3"
            );
            transaction.exec0(sql);

            sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}) VALUES ('{}', '{}', '{}', '{}', '{}')",
                ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::BODY_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CREATION_TIME_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                ContentMetadataRepository::DatabaseKeys::USER_NAME_COLUMN_NAME,
                boost::uuids::to_string(boost::uuids::random_generator()()),
                "Unrelated Comment",
                formatPgSqlTimestamp(a_time_point),
                boost::uuids::to_string(m_content_id_2),
                "unrelated_user"
            );
            transaction.exec0(sql);
            transaction.commit();
        }
    };

}
