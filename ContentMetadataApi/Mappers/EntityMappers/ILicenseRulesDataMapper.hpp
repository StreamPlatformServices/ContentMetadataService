#pragma once
#include "../../StdAfx.hpp"
#include "../../DataTransferObjects/LicenseRulesDto.hpp"
#include "../../../ContentMetadataCore/Entities/LicenseRules.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{
		
		class ILicenseRulesDataMapper
		{
		public:
			virtual ~ILicenseRulesDataMapper() = default;

			virtual auto licenseRulesEntityFrom(const Dto::LicenseRulesDto& a_license_rules_dto) const -> ContentMetadataCore::Entities::LicenseRules = 0;
			virtual auto licenseRulesDtoFrom(const ContentMetadataCore::Entities::LicenseRules& a_license_rules_entity) const -> Dto::LicenseRulesDto = 0;

		};
	}
}