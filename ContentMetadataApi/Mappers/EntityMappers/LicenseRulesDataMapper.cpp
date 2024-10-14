#pragma once
#include "LicenseRulesDataMapper.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{        
        auto LicenseRulesDataMapper::licenseRulesEntityFrom(const Dto::LicenseRulesDto& a_license_rules_dto) const -> ContentMetadataCore::Entities::LicenseRules
        {
            ContentMetadataCore::Entities::LicenseRules license_rules_entity;

            license_rules_entity.m_uuid = a_license_rules_dto.m_uuid;
            license_rules_entity.m_price = a_license_rules_dto.m_price;
            license_rules_entity.m_type = a_license_rules_dto.m_type;
            license_rules_entity.m_duration = a_license_rules_dto.m_duration;

            return license_rules_entity;
        }

        auto LicenseRulesDataMapper::licenseRulesDtoFrom(const ContentMetadataCore::Entities::LicenseRules& a_license_rules_entity) const -> Dto::LicenseRulesDto
        {
            Dto::LicenseRulesDto license_rules_dto;

            license_rules_dto.m_uuid = a_license_rules_entity.m_uuid;
            license_rules_dto.m_price = a_license_rules_entity.m_price;
            license_rules_dto.m_type = a_license_rules_entity.m_type;
            license_rules_dto.m_duration = a_license_rules_entity.m_duration;

            return license_rules_dto;
        }
	}
}