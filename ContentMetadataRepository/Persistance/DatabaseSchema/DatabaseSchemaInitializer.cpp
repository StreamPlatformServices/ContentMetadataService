#include "DatabaseSchemaInitializer.hpp"
#include "../../Constants/DatabaseKeys.hpp"


namespace ContentMetadataRepository 
{
    DatabaseSchemaInitializer::DatabaseSchemaInitializer(std::shared_ptr<ContentMetadataCommon::IConfig> a_configuration) noexcept
        : m_configuration(a_configuration)
    {
    }

    void DatabaseSchemaInitializer::createContentMetadataTables() const
    {
        try
        {
            pqxx::connection database_connection(std::format(
                "dbname={} user={} password={}",
                m_configuration->getDbName(), m_configuration->getDbUsername(), m_configuration->getDbPassword()));

            pqxx::work transaction(database_connection);

            createContentTable(transaction);
            createContentCommentTable(transaction);
            createLicenseRuleTable(transaction);

            transaction.commit();
        }
        catch (const pqxx::broken_connection& e)
        {
            std::cerr << "Connection to database: " << '"' << m_configuration->getDbName() << '"' << " failed." << std::endl;
            throw;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Operation createContentMetadataTables failed." << std::endl;
            throw;
        }
    }


    void DatabaseSchemaInitializer::createContentTable(pqxx::work& a_transaction) const
    {
        std::string sql = std::format(
            R"(
            CREATE TABLE IF NOT EXISTS {} (
                {} UUID PRIMARY KEY,
                {} VARCHAR(255) NOT NULL,
                {} TIMESTAMP NOT NULL,
                {} INTEGER NOT NULL,
                {} TEXT NOT NULL,
                {} UUID,
                {} UUID,
                {} UUID
            );
            )",
            DatabaseKeys::CONTENT_TABLE_NAME,
            DatabaseKeys::UUID_COLUMN_NAME,
            DatabaseKeys::TITLE_COLUMN_NAME,
            DatabaseKeys::UPLOAD_TIME_COLUMN_NAME,
            DatabaseKeys::DURATION_COLUMN_NAME,
            DatabaseKeys::DESCRIPTION_COLUMN_NAME,
            DatabaseKeys::OWNER_ID_COLUMN_NAME,
            DatabaseKeys::VIDEO_FILE_ID_COLUMN_NAME,
            DatabaseKeys::IMAGE_FILE_ID_COLUMN_NAME
        );

        try 
        {
            a_transaction.exec0(sql);
        }
        catch (const pqxx::sql_error& e) 
        {
            std::cerr << "SQL error: " << e.what() << std::endl;
            std::cerr << "Query: " << e.query() << std::endl;  
        }
        catch (const std::exception& e) 
        {
            std::cerr << "Error: " << e.what() << std::endl;  
        }
    }

    void DatabaseSchemaInitializer::createLicenseRuleTable(pqxx::work& a_transaction) const
    {
        std::string sql = std::format(
            R"(
                CREATE TABLE IF NOT EXISTS {} (
                    {} UUID PRIMARY KEY,
                    {} TEXT NOT NULL,
                    {} VARCHAR(50) NOT NULL,
                    {} INTEGER NOT NULL,
                    {} UUID NOT NULL,
                    FOREIGN KEY ({}) REFERENCES {}({}) ON DELETE CASCADE
                );
            )",
           DatabaseKeys::LICENSE_RULES_TABLE_NAME,
           DatabaseKeys::UUID_COLUMN_NAME,
           DatabaseKeys::PRICE_COLUMN_NAME,
           DatabaseKeys::LICENSE_TYPE_COLUMN_NAME,
           DatabaseKeys::LICENSE_DURATION_COLUMN_NAME,
           DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
           DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
           DatabaseKeys::CONTENT_TABLE_NAME, DatabaseKeys::UUID_COLUMN_NAME
        );

        try
        {
            a_transaction.exec0(sql);
        }
        catch (const pqxx::sql_error& e)
        {
            std::cerr << "SQL error: " << e.what() << std::endl;
            std::cerr << "Query: " << e.query() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
      
    }

    void DatabaseSchemaInitializer::createContentCommentTable(pqxx::work& a_transaction) const
    {
        std::string sql = std::format(
            R"(
                CREATE TABLE IF NOT EXISTS {} (
                    {} UUID PRIMARY KEY,
                    {} TEXT NOT NULL,
                    {} TIMESTAMP NOT NULL,
                    {} UUID NOT NULL,
                    {} VARCHAR(255) NOT NULL,
                    FOREIGN KEY ({}) REFERENCES {}({}) ON DELETE CASCADE
                );
            )",
            DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
            DatabaseKeys::UUID_COLUMN_NAME,
            DatabaseKeys::BODY_COLUMN_NAME,
            DatabaseKeys::CREATION_TIME_COLUMN_NAME,
            DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
            DatabaseKeys::USER_NAME_COLUMN_NAME,
            DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
            DatabaseKeys::CONTENT_TABLE_NAME, DatabaseKeys::UUID_COLUMN_NAME
        );

        try
        {
            a_transaction.exec0(sql);
        }
        catch (const pqxx::sql_error& e)
        {
            std::cerr << "SQL error: " << e.what() << std::endl;
            std::cerr << "Query: " << e.query() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

} // namespace ContentMetadataRepository

