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

			virtual ContentMetadataCore::Entities::LicenseRules licenseRulesEntityFrom(const Dto::LicenseRulesDto& a_license_rules_dto) const = 0;
			virtual Dto::LicenseRulesDto licenseRulesDtoFrom(const ContentMetadataCore::Entities::LicenseRules& a_license_rules_entity) const = 0;

		};
	}
}