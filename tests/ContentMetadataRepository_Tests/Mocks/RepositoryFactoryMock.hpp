#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataRepository/Persistance/Repositories/IRepositoryFactory.hpp"

namespace ContentMetadataRepository_Tests
{
    namespace Mocks
    {
        class RepositoryFactoryMock : public ContentMetadataRepository::IRepositoryFactory
        {
        public:
            MOCK_METHOD(std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>, createContentCommentRepository, (std::shared_ptr<pqxx::work> a_transaction), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataRepository::IContentRepository>, createContentRepository, (std::shared_ptr<pqxx::work> a_transaction), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>, createLicenseRulesRepository, (std::shared_ptr<pqxx::work> a_transaction), (const, override));
        };
    }
}
