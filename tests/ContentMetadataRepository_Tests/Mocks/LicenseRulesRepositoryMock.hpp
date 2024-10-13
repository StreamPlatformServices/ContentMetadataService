#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataRepository/Persistance/Repositories/LicenseRules/ILicenseRulesRepository.hpp"

namespace ContentMetadataRepository_Tests
{
    namespace Mocks
    {
        class LicenseRulesRepositoryMock : public ContentMetadataRepository::ILicenseRulesRepository
        {
        public:
            MOCK_METHOD(boost::asio::awaitable<boost::uuids::uuid>, addAsync, (ContentMetadataCore::Entities::LicenseRules& a_license_rules), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, updateAsync, (const ContentMetadataCore::Entities::LicenseRules& a_license_rules), (override));
            MOCK_METHOD(boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::LicenseRules>>, getByContentIdAsync, (const boost::uuids::uuid& a_content_id), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, deleteByContentIdAsync, (const boost::uuids::uuid& a_content_id), (override));
        };
    }
}
