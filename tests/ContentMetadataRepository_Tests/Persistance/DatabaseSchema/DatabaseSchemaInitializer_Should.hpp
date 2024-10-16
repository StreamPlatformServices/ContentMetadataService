#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../ContentMetadataRepository/Persistance/DatabaseSchema/DatabaseSchemaInitializer.hpp"
#include "../../../../ContentMetadataCommon/Configuration/Config.hpp"
#include "../../../../ContentMetadataRepository/Constants/DatabaseKeys.hpp"

namespace ContentMetadataRepository_Tests
{
    class DatabaseSchemaInitializer_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<ContentMetadataCommon::Config> m_config;
        std::shared_ptr<pqxx::connection> m_connection;

        void SetUp() override
        {
            m_config = std::make_shared<ContentMetadataCommon::Config>("config.ini");

            std::string conn_str = std::format(
                "dbname={} user={} password={} host={} port={}",
                m_config->getDbName(),
                m_config->getDbUsername(),
                m_config->getDbPassword(),
                m_config->getDbHost(),
                m_config->getDbPort()
            );
            m_connection = std::make_shared<pqxx::connection>(conn_str);
        }

        void TearDown() override
        {
            pqxx::work transaction(*m_connection);
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME) + " CASCADE;");
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_TABLE_NAME) + " CASCADE;");
            transaction.exec0("DROP TABLE IF EXISTS " + std::string(ContentMetadataRepository::DatabaseKeys::LICENSE_RULES_TABLE_NAME) + " CASCADE;");
            transaction.commit();
        }
    };
}
