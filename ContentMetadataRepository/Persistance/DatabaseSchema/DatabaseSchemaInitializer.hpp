#pragma once
#include "../../StdAfx.hpp"
#include "../../../ContentMetadataCommon/Configuration/IConfig.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT DatabaseSchemaInitializer
    {
    public:
        explicit DatabaseSchemaInitializer(std::shared_ptr<ContentMetadataCommon::IConfig> a_configuration) noexcept;

        DatabaseSchemaInitializer(const DatabaseSchemaInitializer&) = delete;
        DatabaseSchemaInitializer(DatabaseSchemaInitializer&&) = delete;

        DatabaseSchemaInitializer& operator=(const DatabaseSchemaInitializer&) = delete;
        DatabaseSchemaInitializer& operator=(DatabaseSchemaInitializer&&) = delete;

        void createContentMetadataTables() const;

    private:
        void createContentTable(pqxx::work& a_transaction) const;
        void createContentCommentTable(pqxx::work& a_transaction) const;
        void createLicenseRuleTable(pqxx::work& a_transaction) const;

    private:
        std::shared_ptr<ContentMetadataCommon::IConfig> m_configuration;
    };

}