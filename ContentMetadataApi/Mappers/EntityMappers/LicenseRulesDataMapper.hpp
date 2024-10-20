#pragma once
#include "ILicenseRulesDataMapper.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{
		class CONTENT_METADATA_API_EXPORT LicenseRulesDataMapper : public ILicenseRulesDataMapper
		{
		public:
			LicenseRulesDataMapper() = default;
			LicenseRulesDataMapper(const LicenseRulesDataMapper&) = delete;
			LicenseRulesDataMapper(LicenseRulesDataMapper&&) = delete;

			LicenseRulesDataMapper& operator=(const LicenseRulesDataMapper&) = delete;
			LicenseRulesDataMapper& operator=(LicenseRulesDataMapper&&) = delete;

			auto licenseRulesEntityFrom(const Dto::LicenseRulesDto& a_license_rules_dto) const -> ContentMetadataCore::Entities::LicenseRules override;
			auto licenseRulesDtoFrom(const ContentMetadataCore::Entities::LicenseRules& a_license_rules_entity) const -> Dto::LicenseRulesDto override;
		};
	}
}