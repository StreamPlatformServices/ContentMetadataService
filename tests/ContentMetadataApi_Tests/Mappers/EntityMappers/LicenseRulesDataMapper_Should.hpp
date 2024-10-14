#pragma once
#include <gtest/gtest.h>
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/ILicenseRulesDataMapper.hpp"
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/LicenseRulesDataMapper.hpp"
#include "../../../../ContentMetadataCore/Entities/LicenseRules.hpp"
#include "../../../../ContentMetadataApi/DataTransferObjects/LicenseRulesDto.hpp"
#include <boost/uuid/string_generator.hpp>
#include <chrono>
#include <memory>

namespace ContentMetadataApi_Tests
{
    class LicenseRulesDataMapper_Should : public ::testing::TestWithParam<std::tuple<
        std::string,       // UUID
        int,               // Price
        ContentMetadataCore::Enums::LicenseType, // License type
        std::optional<ContentMetadataCore::Enums::LicenseDuration>, // License duration
        std::string        // UUID
        >>
    {
    protected:
        std::unique_ptr<ContentMetadataApi::Mappers::ILicenseRulesDataMapper> m_sut = std::make_unique<ContentMetadataApi::Mappers::LicenseRulesDataMapper>();
    };
}