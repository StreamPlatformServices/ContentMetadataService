#pragma once
#include "../../../StdAfx.hpp"
#include "../../../../ContentMetadataCore/Entities/LicenseRules.hpp"

namespace ContentMetadataRepository
{
    class ILicenseRulesRepository
    {
    public:
        virtual ~ILicenseRulesRepository() = default;

        virtual auto addAsync(ContentMetadataCore::Entities::LicenseRules& a_license_rules) -> boost::asio::awaitable<boost::uuids::uuid> = 0;
        virtual auto updateAsync(const ContentMetadataCore::Entities::LicenseRules& a_license_rules) -> boost::asio::awaitable<void> = 0;
        virtual auto getByContentIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::LicenseRules>> = 0;
        virtual auto deleteByContentIdAsync(const boost::uuids::uuid& a_content_id)->boost::asio::awaitable<void> = 0;
    };
}
