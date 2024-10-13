#include "DatabaseSchemaInitializer_Should.hpp"
#include "../../../../ContentMetadataRepository/Constants/DatabaseKeys.hpp"


namespace ContentMetadataRepository_Tests 
{
    TEST_F(DatabaseSchemaInitializer_Should, CreateTablesSuccessfully)
    {
        ContentMetadataRepository::DatabaseSchemaInitializer schema_initializer(m_config);

        schema_initializer.createContentMetadataTables();

        pqxx::work transaction(*m_connection);

        auto content_res = transaction.exec("SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = '" + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_TABLE_NAME) + "')");
        EXPECT_EQ(content_res[0][0].as<bool>(), true);

        auto comment_res = transaction.exec("SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = '" + std::string(ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME) + "')");
        EXPECT_EQ(comment_res[0][0].as<bool>(), true);

        auto license_res = transaction.exec("SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = '" + std::string(ContentMetadataRepository::DatabaseKeys::LICENSE_RULES_TABLE_NAME) + "')");
        EXPECT_EQ(license_res[0][0].as<bool>(), true);

        transaction.commit();
    }

} // namespace ContentMetadataRepository_Tests

