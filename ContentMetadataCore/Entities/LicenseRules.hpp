#pragma once
#include "../StdAfx.hpp"
#include "../Enums/LicenseDuration.hpp"
#include "../Enums/LicenseType.hpp"

namespace ContentMetadataCore
{
    namespace Entities
    {
        struct LicenseRules
        {
            boost::uuids::uuid m_uuid = boost::uuids::nil_uuid();
            int m_price = 0;
            Enums::LicenseType m_type = Enums::LicenseType::Unknown;
            std::optional<Enums::LicenseDuration> m_duration;
            boost::uuids::uuid m_content_id = boost::uuids::nil_uuid();
        };
    }
}