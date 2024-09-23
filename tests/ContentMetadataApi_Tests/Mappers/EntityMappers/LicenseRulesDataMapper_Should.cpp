#pragma once
#include "LicenseRulesDataMapper_Should.hpp"

TEST_P(LicenseRulesDataMapper_Should, ConvertDtoToEntityCorrectly) {
    auto [uuid_str, price, license_type, license_duration] = GetParam();

    ContentMetadataApi::Dto::LicenseRulesDto dto;
    dto.m_uuid = boost::uuids::string_generator()(uuid_str);
    dto.m_price = price;
    dto.m_type = license_type;
    dto.m_duration = license_duration;

    ContentMetadataCore::Entities::LicenseRules expected_entity;
    expected_entity.m_uuid = boost::uuids::string_generator()(uuid_str);
    expected_entity.m_price = price;
    expected_entity.m_type = license_type;
    expected_entity.m_duration = license_duration;

    auto actual_entity = m_sut->licenseRulesEntityFrom(dto);

    EXPECT_EQ(boost::uuids::to_string(expected_entity.m_uuid), boost::uuids::to_string(actual_entity.m_uuid));
    EXPECT_EQ(expected_entity.m_price, actual_entity.m_price);
    EXPECT_EQ(expected_entity.m_type, actual_entity.m_type);
    EXPECT_EQ(expected_entity.m_duration, actual_entity.m_duration);
}

TEST_P(LicenseRulesDataMapper_Should, ConvertEntityToDtoCorrectly) {
    auto [uuid_str, price, license_type, license_duration] = GetParam();

    ContentMetadataCore::Entities::LicenseRules entity;
    entity.m_uuid = boost::uuids::string_generator()(uuid_str);
    entity.m_price = price;
    entity.m_type = license_type;
    entity.m_duration = license_duration;

    ContentMetadataApi::Dto::LicenseRulesDto expected_dto;
    expected_dto.m_uuid = boost::uuids::string_generator()(uuid_str);
    expected_dto.m_price = price;
    expected_dto.m_type = license_type;
    expected_dto.m_duration = license_duration;

    auto actual_dto = m_sut->licenseRulesDtoFrom(entity);

    EXPECT_EQ(boost::uuids::to_string(expected_dto.m_uuid), boost::uuids::to_string(actual_dto.m_uuid));
    EXPECT_EQ(expected_dto.m_price, actual_dto.m_price);
    EXPECT_EQ(expected_dto.m_type, actual_dto.m_type);
    EXPECT_EQ(expected_dto.m_duration, actual_dto.m_duration);
}

INSTANTIATE_TEST_SUITE_P(
    LicenseRulesMapperTests,
    LicenseRulesDataMapper_Should,
    ::testing::Values(
        std::make_tuple(
            "00000000-0000-0000-0000-000000000000",  // UUID
            100,                                    // Price
            ContentMetadataCore::Enums::LicenseType::Rent,  // License type
            std::optional<ContentMetadataCore::Enums::LicenseDuration>(ContentMetadataCore::Enums::LicenseDuration::OneDay)  // Duration 
        ),
        std::make_tuple(
            "11223344-5566-7788-99aa-bbccddeeff00",  // UUID
            500,                                    // Price
            ContentMetadataCore::Enums::LicenseType::Buy,  // License type
            std::nullopt                            // No duration (permanent license)
        ),
        std::make_tuple(
            "22334455-6677-8899-aabb-ccddeeff0011",  // UUID
            200,                                    // Price
            ContentMetadataCore::Enums::LicenseType::Rent,  // License type
            std::optional<ContentMetadataCore::Enums::LicenseDuration>(ContentMetadataCore::Enums::LicenseDuration::TwoDays)  // Duration 
        ),
        std::make_tuple(
            "33445566-7788-99aa-bbcc-ddeeff002233",  // UUID
            300,                                    // Price
            ContentMetadataCore::Enums::LicenseType::Rent,  // License type
            std::optional<ContentMetadataCore::Enums::LicenseDuration>(ContentMetadataCore::Enums::LicenseDuration::ThreeDays)  // Duration 
        ),
        std::make_tuple(
            "44556677-8899-aabb-ccdd-eeff00334455",  // UUID
            400,                                    // Price
            ContentMetadataCore::Enums::LicenseType::Rent,  // License type
            std::optional<ContentMetadataCore::Enums::LicenseDuration>(ContentMetadataCore::Enums::LicenseDuration::Week)  // Duration 
        ),
        std::make_tuple(
            "55667788-99aa-bbcc-ddee-ff0044556677",  // UUID
            8760,                                   // Price 
            ContentMetadataCore::Enums::LicenseType::Rent,  // License type
            std::optional<ContentMetadataCore::Enums::LicenseDuration>(ContentMetadataCore::Enums::LicenseDuration::Month)  // Duration 
        )
    )
);

