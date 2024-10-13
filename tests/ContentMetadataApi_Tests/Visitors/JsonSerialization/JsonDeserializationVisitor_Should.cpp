#pragma once
#include "JsonDeserializationVisitor_Should.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_P(JsonDeserializationVisitorContentDto_Should, DeserializeContentDto)
    {
        auto [uuid_str, title, description, duration, owner_id, video_file_id, image_file_id, upload_time] = GetParam();

        auto license_rule_uuid_str = std::string("44ee6677-8899-aabb-ccdd-eeff0f334455");
        auto comment_uuid_str = std::string("123e4567-e89b-12d3-a456-426614174000");
        auto comment_creation_time = std::string("2024-09-25T15:30:00Z");
        auto comment_user_name = std::string("john_doe");

        std::string license_rules_json = R"([{
        "uuid": ")" + license_rule_uuid_str + R"(",
        "price": 50,
        "type": 2,
        "duration": 1
    }
    ])";

        std::string content_comments_json = R"([{
        "uuid": ")" + comment_uuid_str + R"(",
        "body": "This is a comment on the content.",
        "creation_time": ")" + comment_creation_time + R"(",
        "content_id": ")" + uuid_str + R"(",
        "user_name": ")" + comment_user_name + R"("
    }
    ])";

        auto json_input = R"({
        "uuid": ")" + uuid_str + R"(",
        "title": ")" + title + R"(",
        "description": ")" + description + R"(",
        "duration": )" + std::to_string(duration) + R"(,
        "owner_id": ")" + owner_id + R"(",
        "video_file_id": ")" + video_file_id + R"(",
        "image_file_id": ")" + image_file_id + R"(",
        "upload_time": ")" + upload_time + R"(",
        "license_rules": )" + license_rules_json + R"(,
        "content_comments": )" + content_comments_json + R"(
    })";

        auto sut = m_visitor_factory->createJsonDeserializationVisitor(json_input);

        ContentMetadataApi::Dto::ContentDto content_dto;

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(uuid_str))
            .WillOnce(::testing::Return(boost::uuids::string_generator()(uuid_str)))
            .WillOnce(::testing::Return(boost::uuids::string_generator()(uuid_str)));

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(license_rule_uuid_str))
            .WillOnce(::testing::Return(boost::uuids::string_generator()(license_rule_uuid_str)));

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(comment_uuid_str))
            .WillOnce(::testing::Return(boost::uuids::string_generator()(comment_uuid_str)));

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(owner_id))
            .WillOnce(::testing::Return(boost::uuids::string_generator()(owner_id)));

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(video_file_id))
            .WillOnce(::testing::Return(boost::uuids::string_generator()(video_file_id)));

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(image_file_id))
            .WillOnce(::testing::Return(boost::uuids::string_generator()(image_file_id)));

        ::testing::Sequence s1, s2;

        EXPECT_CALL(*m_mock_date_time_parser, parseIso8601(upload_time)).Times(1).InSequence(s1);
        EXPECT_CALL(*m_mock_date_time_parser, getTimePoint())
            .InSequence(s1)
            .WillOnce(::testing::Return(stringToTimePoint(upload_time)));

        EXPECT_CALL(*m_mock_date_time_parser, parseIso8601(comment_creation_time)).Times(1).InSequence(s2);
        EXPECT_CALL(*m_mock_date_time_parser, getTimePoint())
            .InSequence(s2)
            .WillOnce(::testing::Return(stringToTimePoint(comment_creation_time)));

        sut->visit(content_dto);

        EXPECT_EQ(content_dto.m_uuid, boost::uuids::string_generator()(uuid_str));
        EXPECT_EQ(content_dto.m_title, title);
        EXPECT_EQ(content_dto.m_description, description);
        EXPECT_EQ(content_dto.m_duration, duration);

        ASSERT_EQ(content_dto.m_license_rules.size(), 1);
        const auto& license_rule = content_dto.m_license_rules[0];
        EXPECT_EQ(license_rule.m_uuid, boost::uuids::string_generator()(license_rule_uuid_str));
        EXPECT_EQ(license_rule.m_price, 50);
        EXPECT_EQ(static_cast<int>(license_rule.m_type), static_cast<int>(ContentMetadataCore::Enums::LicenseType::Rent));
        EXPECT_EQ(static_cast<int>(license_rule.m_duration.value()), static_cast<int>(ContentMetadataCore::Enums::LicenseDuration::OneDay));

        ASSERT_EQ(content_dto.m_content_comments->size(), 1);
        const auto& comment = (*content_dto.m_content_comments)[0];
        EXPECT_EQ(comment.m_uuid, boost::uuids::string_generator()(comment_uuid_str));
        EXPECT_EQ(comment.m_body, "This is a comment on the content.");
        EXPECT_EQ(comment.m_creation_time, stringToTimePoint(comment_creation_time));
        EXPECT_EQ(comment.m_content_id, boost::uuids::string_generator()(uuid_str));
        EXPECT_EQ(comment.m_user_name, comment_user_name);
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentDtoTests,
        JsonDeserializationVisitorContentDto_Should,
        ::testing::Values(

            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Sample Title", "Sample Description", 120,
                "11223344-5566-7788-99aa-bbccddeeff01", "11223344-5566-7788-99aa-bbccddeeff02", "11223344-5566-7788-99aa-bbccddeeff03", "2023-09-25T10:15:30Z"),

            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", "Another Title", "Another Description", 60,
                "22334455-6677-8899-aabb-ccddeeff0012", "22334455-6677-8899-aabb-ccddeeff0013", "22334455-6677-8899-aabb-ccddeeff0014", "2023-09-24T08:45:00Z"),

            std::make_tuple("33445566-7788-99aa-bbcc-ddeeff002233", "", "No Title Description", 180,
                "33445566-7788-99aa-bbcc-ddeeff002234", "33445566-7788-99aa-bbcc-ddeeff002235", "33445566-7788-99aa-bbcc-ddeeff002236", "2023-09-23T12:30:00Z"),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "Title With Empty Description", "", 240,
                "44556677-8899-aabb-ccdd-eeff00334456", "44556677-8899-aabb-ccdd-eeff00334457", "44556677-8899-aabb-ccdd-eeff00334458", "2023-09-22T11:00:00Z"),

            std::make_tuple("55667788-99aa-bbcc-ddee-ff0044556677", "Zero Duration Title", "This content has zero duration", 0,
                "55667788-99aa-bbcc-ddee-ff0044556678", "55667788-99aa-bbcc-ddee-ff0044556679", "55667788-99aa-bbcc-ddee-ff0044556680", "2023-09-21T09:15:00Z"),

            std::make_tuple("66778899-aabb-ccdd-eeff-005566778899", "Long Duration Title", "This content has a long duration", 10000,
                "66778899-aabb-ccdd-eeff-005566778900", "66778899-aabb-ccdd-eeff-005566778901", "66778899-aabb-ccdd-eeff-005566778902", "2023-09-20T15:30:00Z")
        )
    );

    TEST_P(JsonDeserializationVisitorContentDtoInvalid_Should, ShouldThrowInvalidArgumentOnInvalidGuid)
    {
        auto [uuid_str, title, description, duration, owner_id, video_file_id, image_file_id, upload_time] = GetParam();

        auto license_rule_uuid_str = std::string("44ee6677-8899-aabb-ccdd-eeff0f334455");

        std::string license_rules_json = R"([{
        "uuid": ")" + license_rule_uuid_str + R"(",
        "price": 50,
        "type": 2,
        "duration": 1
    }
    ])";

        auto json_input = R"({
        "uuid": ")" + uuid_str + R"(",
        "title": ")" + title + R"(",
        "description": ")" + description + R"(",
        "duration": )" + std::to_string(duration) + R"(,
        "owner_id": ")" + owner_id + R"(",
        "video_file_id": ")" + video_file_id + R"(",
        "image_file_id": ")" + image_file_id + R"(",
        "upload_time": ")" + upload_time + R"(",
        "license_rules": )" + license_rules_json + R"(
    })";

        auto sut = m_visitor_factory->createJsonDeserializationVisitor(json_input);

        m_mock_guid_parser->delegateToReal();

        ContentMetadataApi::Dto::ContentDto content_dto;

        EXPECT_THROW(sut->visit(content_dto), std::invalid_argument);
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentDtoInvalidTests,
        JsonDeserializationVisitorContentDtoInvalid_Should,
        ::testing::Values(
            std::make_tuple("778899aa-bbcc-ddee-eeff-ff0066778899", "No Owner Title", "Missing owner_id field", 180,
                "", "778899aa-bbcc-ddee-eeff-ff0066778899", "778899aa-bbcc-ddee-eeff-ff0066778898", "2023-09-19T18:00:00Z"),

            std::make_tuple("8899aabb-ccdd-eeff-ff00778899aabb", "No Video Title", "Missing video_file_id field", 180,
                "8899aabb-ccdd-eeff-ff00778899aabc", "", "8899aabb-ccdd-eeff-ff00778899aabd", "2023-09-18T14:45:00Z"),

            std::make_tuple("99aabbcc-ddee-ff00-7788-99aabbccddeeff", "No Image Title", "Missing image_file_id field", 180,
                "99aabbcc-ddee-ff00-7788-99aabbccddeeff", "99aabbcc-ddee-ff00-7788-99aabbccddeeff", "", "2023-09-17T10:20:00Z")
        )
    );




    TEST_P(JsonDeserializationVisitorContentCommentDto_Should, DeserializeContentCommentDto)
    {
        auto [uuid_str, body, user_name, creation_time, content_id_str] = GetParam();

        auto json_input = R"({
            "uuid": ")" + uuid_str + R"(",
            "body": ")" + body + R"(",
            "user_name": ")" + user_name + R"(",
            "creation_time": ")" + creation_time + R"(",
            "content_id": ")" + content_id_str + R"("
        })";

        auto sut = m_visitor_factory->createJsonDeserializationVisitor(json_input);

        ContentMetadataApi::Dto::ContentCommentDto comment_dto;

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(uuid_str)).WillOnce(::testing::Return(boost::uuids::string_generator()(uuid_str)));
        EXPECT_CALL(*m_mock_guid_parser, parseGuid(content_id_str)).WillOnce(::testing::Return(boost::uuids::string_generator()(content_id_str)));

        EXPECT_CALL(*m_mock_date_time_parser, parseIso8601(creation_time)).Times(1);
        EXPECT_CALL(*m_mock_date_time_parser, getTimePoint()).WillOnce(::testing::Return(stringToTimePoint(creation_time)));

        sut->visit(comment_dto);

        EXPECT_EQ(comment_dto.m_uuid, boost::uuids::string_generator()(uuid_str));
        EXPECT_EQ(comment_dto.m_body, body);
        EXPECT_EQ(comment_dto.m_user_name, user_name);
        EXPECT_EQ(comment_dto.m_content_id, boost::uuids::string_generator()(content_id_str));
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentCommentDtoTests,
        JsonDeserializationVisitorContentCommentDto_Should,
        ::testing::Values(
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Sample body", "SampleUser", "2023-09-25T10:15:30Z", "22334455-6677-8899-aabb-ccddeeff0011"),
            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", "", "UserWithoutBody", "2023-09-25T10:15:30Z", "33445566-7788-99aa-bbcc-ddeeff002233")
        )
    );


    TEST_P(JsonDeserializationVisitorContentCommentDtoInvalid_Should, ShouldThrowInvalidArgumentOnInvalidInput)
    {
        auto [uuid_str, body, user_name, creation_time, content_id_str] = GetParam();

        auto json_input = R"({
        "uuid": ")" + uuid_str + R"(",
        "body": ")" + body + R"(",
        "user_name": ")" + user_name + R"(",
        "creation_time": ")" + creation_time + R"(",
        "content_id": ")" + content_id_str + R"(" })";

        auto sut = m_visitor_factory->createJsonDeserializationVisitor(json_input);

        ContentMetadataApi::Dto::ContentCommentDto comment_dto;

        m_mock_guid_parser->delegateToReal();
        m_mock_date_time_parser->delegateToReal();

        EXPECT_THROW(sut->visit(comment_dto), std::invalid_argument);
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentCommentDtoInvalidTests,
        JsonDeserializationVisitorContentCommentDtoInvalid_Should,
        ::testing::Values(
            std::make_tuple("invalid-uuid", "Sample body", "SampleUser", "2023-09-25T10:15:30Z", "11223344-5566-7788-99aa-bbccddeeff00"),
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Sample body", "SampleUser", "invalid-time", "11223344-5566-7788-99aa-bbccddeeff00"),
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Sample body", "SampleUser", "2023-09-25T10:15:30Z", "invalid-content-id")
        )
    );




    TEST_P(JsonDeserializationVisitorLicenseRulesDto_Should, DeserializeLicenseRulesDto)
    {
        auto [uuid_str, price, type, duration] = GetParam();

        std::string duration_field = duration.has_value() ? std::to_string(static_cast<int>(duration.value())) : "null";
        auto json_input = R"({
        "uuid": ")" + uuid_str + R"(",
        "price": )" + std::to_string(price) + R"(,
        "type": )" + std::to_string(static_cast<int>(type)) + R"(,
        "duration": )" + duration_field + R"(
    })";

        auto sut = m_visitor_factory->createJsonDeserializationVisitor(json_input);

        ContentMetadataApi::Dto::LicenseRulesDto license_rules_dto;

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(uuid_str)).WillOnce(::testing::Return(boost::uuids::string_generator()(uuid_str)));

        sut->visit(license_rules_dto);

        EXPECT_EQ(license_rules_dto.m_uuid, boost::uuids::string_generator()(uuid_str));
        EXPECT_EQ(license_rules_dto.m_price, price);
        EXPECT_EQ(license_rules_dto.m_type, type);
        if (duration.has_value())
        {
            EXPECT_EQ(license_rules_dto.m_duration.value(), duration.value());
        }
        else
        {
            EXPECT_FALSE(license_rules_dto.m_duration.has_value());
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        LicenseRulesDtoTests,
        JsonDeserializationVisitorLicenseRulesDto_Should,
        ::testing::Values(
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", 100, ContentMetadataCore::Enums::LicenseType::Rent,
                ContentMetadataCore::Enums::LicenseDuration::OneDay),
            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", 50, ContentMetadataCore::Enums::LicenseType::Unknown,
                ContentMetadataCore::Enums::LicenseDuration::TwoDays)
        )
    );

    TEST_P(JsonDeserializationVisitorLicenseRulesDtoInvalid_Should, ShouldThrowInvalidArgumentOnInvalidInput)
    {
        auto [uuid_str, price, type, duration] = GetParam();

        std::string duration_field = duration.has_value() ? std::to_string(static_cast<int>(duration.value())) : "null";
        auto json_input = R"({
            "uuid": ")" + uuid_str + R"(",
            "price": )" + std::to_string(price) + R"(,
            "type": )" + std::to_string(static_cast<int>(type)) + R"(,
            "duration": )" + duration_field + R"(
        })";

        auto sut = m_visitor_factory->createJsonDeserializationVisitor(json_input);

        ContentMetadataApi::Dto::LicenseRulesDto license_rules_dto;

        m_mock_guid_parser->delegateToReal();

        EXPECT_THROW(sut->visit(license_rules_dto), std::invalid_argument);
    }

    INSTANTIATE_TEST_SUITE_P(
        LicenseRulesDtoInvalidTests,
        JsonDeserializationVisitorLicenseRulesDtoInvalid_Should,
        ::testing::Values(
            std::make_tuple("invalid-uuid", 100, ContentMetadataCore::Enums::LicenseType::Rent,
                ContentMetadataCore::Enums::LicenseDuration::OneDay), // Invalid UUID
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", 100, static_cast<ContentMetadataCore::Enums::LicenseType>(-1),
                ContentMetadataCore::Enums::LicenseDuration::OneDay), // Invalid license type
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", 100, ContentMetadataCore::Enums::LicenseType::Rent,
                static_cast<ContentMetadataCore::Enums::LicenseDuration>(-1)), // Invalid license duration
            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", 50, ContentMetadataCore::Enums::LicenseType::Unknown,
                std::nullopt)
        )
    );


}