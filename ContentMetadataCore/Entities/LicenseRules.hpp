#pragma once

#include "../StdAfx.hpp"
#include "../Enums/LicenseDuration.hpp"
#include "../Enums/LicenseType.hpp"

namespace ContentMetadataCore
{
    namespace Entities
    {
        struct CONTENT_METADATA_CORE_EXPORT LicenseRules
        {
            boost::uuids::uuid m_uuid = boost::uuids::nil_uuid();
            int m_price = 0;
            Enums::LicenseType m_type = Enums::LicenseType::Unknown;
            std::optional<Enums::LicenseDuration> m_duration;
        };
    }
}