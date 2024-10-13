#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataRepository/Persistance/IUnitOfWork.hpp"

namespace ContentMetadataRepository_Tests
{
    namespace Mocks
    {
        class UnitOfWorkMock : public ContentMetadataRepository::IUnitOfWork
        {
        public:
            MOCK_METHOD(boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>, contentRepositoryAsync, (const boost::uuids::uuid&), (override));
            MOCK_METHOD(boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>, contentCommentRepositoryAsync, (const boost::uuids::uuid&), (override));
            MOCK_METHOD(boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>, licenseRulesRepositoryAsync, (const boost::uuids::uuid&), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, commitTransactionAsync, (const boost::uuids::uuid&), (override));
            MOCK_METHOD(void, rollbackTransaction, (const boost::uuids::uuid&), (override));
        };
    }
}
