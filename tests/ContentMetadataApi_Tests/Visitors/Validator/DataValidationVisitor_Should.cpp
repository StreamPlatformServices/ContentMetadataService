#pragma once
#include "DataValidationVisitor_Should.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_P(DataValidationContentDtoVisitor_Should, ValidateContentDto)
    {
        auto [uuid_str, title, duration, description, owner_id_str, video_file_id_str, image_file_id_str, upload_time_str, is_valid] = GetParam();

        ContentMetadataApi::Dto::ContentDto content_dto;
        content_dto.m_uuid = boost::uuids::string_generator()(uuid_str);
        content_dto.m_title = title;
        content_dto.m_duration = duration;
        content_dto.m_description = description;
        content_dto.m_upload_time = stringToTimePoint(upload_time_str);
        content_dto.m_owner_id = boost::uuids::string_generator()(owner_id_str);
        content_dto.m_video_file_id = boost::uuids::string_generator()(video_file_id_str);
        content_dto.m_image_file_id = boost::uuids::string_generator()(image_file_id_str);

        ContentMetadataApi::Dto::LicenseRulesDto license_rule;
        license_rule.m_uuid = boost::uuids::string_generator()("11223344-5566-7788-99aa-bbccddeeff00");
        license_rule.m_price = 50;
        license_rule.m_type = ContentMetadataCore::Enums::LicenseType::Rent;
        license_rule.m_duration = ContentMetadataCore::Enums::LicenseDuration::OneDay;

        content_dto.m_license_rules.push_back(license_rule);

        m_mock_guid_parser->delegateToReal();

        m_sut->visit(content_dto);

        EXPECT_EQ(m_errors.empty(), is_valid);
    }

    INSTANTIATE_TEST_SUITE_P(
        DataValidationContentDtoTests,
        DataValidationContentDtoVisitor_Should,
        ::testing::Values(

            std::make_tuple("00000000-0000-0000-0000-000000000000", "", -1, "Description", "00000000-0000-0000-0000-000000000000", "00000000-0000-0000-0000-000000000000", "00000000-0000-0000-0000-000000000000", "1970-01-01T00:00:00", false),

            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Title 1", 3600, "Description 1", "11223344-5566-7788-99aa-bbccddeeff00", "11223344-5566-7788-99aa-bbccddeeff00", "11223344-5566-7788-99aa-bbccddeeff00", "2023-09-25T10:15:30Z", true),

            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Title 1", 3600, "Description 1", "11223344-5566-7788-99aa-bbccddeeff00", "11223344-5566-7788-99aa-bbccddeeff00", "11223344-5566-7788-99aa-bbccddeeff00", "1970-01-01T00:00:00", false),

            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Title 1", 3600, "Description 1", "11223344-5566-7788-99aa-bbccddeeff00", "11223344-5566-7788-99aa-bbccddeeff00", "11223344-5566-7788-99aa-bbccddeeff00", "2099-01-01T00:00:00", false),

            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", "Title 2", 7200, "Description 2", "00000000-0000-0000-0000-000000000000", "22334455-6677-8899-aabb-ccddeeff0011", "22334455-6677-8899-aabb-ccddeeff0011", "2023-09-24T08:45:00Z", false),

            std::make_tuple("33445566-7788-99aa-bbcc-ddeeff002233", "Title 3", -3600, "Description 3", "33445566-7788-99aa-bbcc-ddeeff002233", "33445566-7788-99aa-bbcc-ddeeff002233", "33445566-7788-99aa-bbcc-ddeeff002233", "2023-09-23T12:30:00Z", false),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "", 5400, "Description 4", "44556677-8899-aabb-ccdd-eeff00334455", "44556677-8899-aabb-ccdd-eeff00334455", "44556677-8899-aabb-ccdd-eeff00334455", "2023-09-22T11:00:00Z", false),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "Title 4", 5400, "", "44556677-8899-aabb-ccdd-eeff00334455", "44556677-8899-aabb-ccdd-eeff00334455", "44556677-8899-aabb-ccdd-eeff00334455", "2023-09-22T11:00:00Z", false),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "Title 5", 5400, "Descpriton 5", "00000000-0000-0000-0000-000000000000", "44556677-8899-aabb-ccdd-eeff00334455", "44556677-8899-aabb-ccdd-eeff00334455", "2023-09-21T09:15:00Z", false),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "Title 6", 5400, "Descpriton 6", "44556677-8899-aabb-ccdd-eeff00334455", "00000000-0000-0000-0000-000000000000", "44556677-8899-aabb-ccdd-eeff00334455", "2023-09-20T15:30:00Z", false),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "Title 7", 5400, "Descpriton 6", "44556677-8899-aabb-ccdd-eeff00334455", "44556677-8899-aabb-ccdd-eeff00334455", "00000000-0000-0000-0000-000000000000", "2023-09-19T12:45:00Z", false)
        )
    );



    TEST_P(DataValidationContentCommentDtoVisitor_Should, ValidateContentCommentDto)
    {
        auto [uuid_str, body, user_name, creation_time_str, uuid_valid, result] = GetParam();

        ContentMetadataApi::Dto::ContentCommentDto comment_dto;
        comment_dto.m_body = body;
        comment_dto.m_user_name = user_name;
        comment_dto.m_creation_time = stringToTimePoint(creation_time_str);

        //Note: EXPECT_CALL needs to have different input values if it is called twice. Either it will be used only on the first call, because it is setup for WillOnce.
        comment_dto.m_content_id = boost::uuids::random_generator()();
        EXPECT_CALL(*m_mock_guid_parser, isGuid(comment_dto.m_uuid)).WillOnce(::testing::Return(uuid_valid));
        EXPECT_CALL(*m_mock_guid_parser, isGuid(comment_dto.m_content_id)).WillOnce(::testing::Return(uuid_valid));

        m_sut->visit(comment_dto);

        EXPECT_EQ(m_errors.empty(), result);
    }

    INSTANTIATE_TEST_SUITE_P(
        DataValidationContentCommentDtoTests,
        DataValidationContentCommentDtoVisitor_Should,
        ::testing::Values(
            std::make_tuple("", "", "", "2023-09-24T08:45:00Z", false, false),
            std::make_tuple("ValidUuid", "Valid Body", "User", "2023-09-24T08:45:00Z", true, true),
            std::make_tuple("ValidUuid", "", "User", "2023-09-24T08:45:00Z", true, false),
            std::make_tuple("ValidUuid", "Valid Body", "", "2023-09-24T08:45:00Z", true, false),
            std::make_tuple("", "Valid Body", "User", "2023-09-24T08:45:00Z", false, false),
            std::make_tuple("ValidUuid", "Valid Body", "User", "2079-09-24T08:45:00Z", true, false),
            std::make_tuple("ValidUuid", "Valid Body", "User", "1970-01-01T00:00:00", true, false)
        )
    );

    TEST_P(DataValidationLicenseRulesDtoVisitor_Should, ValidateLicenseRulesDto)
    {
        auto [price, license_type, duration, uuid_valid] = GetParam();

        ContentMetadataApi::Dto::LicenseRulesDto license_dto;
        license_dto.m_price = price;
        license_dto.m_type = license_type;
        license_dto.m_duration = duration;

        EXPECT_CALL(*m_mock_guid_parser, isGuid(license_dto.m_uuid)).WillOnce(::testing::Return(uuid_valid));

        m_sut->visit(license_dto);

        EXPECT_EQ(m_errors.empty(), uuid_valid && price >= 0 && license_type != ContentMetadataCore::Enums::LicenseType::Unknown);
    }

    INSTANTIATE_TEST_SUITE_P(
        DataValidationLicenseRulesDtoTests,
        DataValidationLicenseRulesDtoVisitor_Should,
        ::testing::Values(
            std::make_tuple(-1, ContentMetadataCore::Enums::LicenseType::Unknown, ContentMetadataCore::Enums::LicenseDuration::Unknown, false),
            std::make_tuple(100, ContentMetadataCore::Enums::LicenseType::Rent, ContentMetadataCore::Enums::LicenseDuration::Month, true),
            std::make_tuple(0, ContentMetadataCore::Enums::LicenseType::Rent, ContentMetadataCore::Enums::LicenseDuration::OneDay, true),
            std::make_tuple(50, ContentMetadataCore::Enums::LicenseType::Buy, ContentMetadataCore::Enums::LicenseDuration::Unknown, true),
            std::make_tuple(75, ContentMetadataCore::Enums::LicenseType::Unknown, ContentMetadataCore::Enums::LicenseDuration::Week, false)
        )
    );

    TEST_P(DataValidationGuidDtoVisitor_Should, ValidateGuidDto)
    {
        auto uuid = GetParam();

        ContentMetadataApi::Dto::GuidDto guid_dto;
        guid_dto.m_uuid = uuid;

        EXPECT_CALL(*m_mock_guid_parser, isGuid(guid_dto.m_uuid)).WillOnce(::testing::Return(boost::uuids::nil_uuid() != uuid));

        m_sut->visit(guid_dto);

        EXPECT_EQ(m_errors.empty(), uuid != boost::uuids::nil_uuid());
    }

    INSTANTIATE_TEST_SUITE_P(
        DataValidationGuidDtoTests,
        DataValidationGuidDtoVisitor_Should,
        ::testing::Values(
            boost::uuids::nil_uuid(),
            boost::uuids::random_generator()(),
            boost::uuids::string_generator()("01234567-89ab-cdef-0123-456789abcdef"),
            boost::uuids::random_generator()(),
            boost::uuids::nil_uuid()
        )
    );
}