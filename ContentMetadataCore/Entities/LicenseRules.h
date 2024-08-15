#pragma once

#include "../StdAfx.h"

namespace ContentMetadataCore
{
    namespace Entities
    {
        enum class LicenseType
        {
            Unknown,
            Buy,
            Rent
        };

        enum class LicenseDuration
        {
            Unknown,
            OneDay,
            TwoDays,
            ThreeDays,
            Week,
            Month
        };

        struct LicenseRules
        {
            boost::uuids::uuid Uuid = boost::uuids::nil_uuid();
            int Price = 0;
            LicenseType Type = LicenseType::Unknown;
            std::optional<LicenseDuration> Duration;
        };
    }
}