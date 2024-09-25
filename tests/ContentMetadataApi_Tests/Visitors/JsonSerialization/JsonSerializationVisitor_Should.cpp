#pragma once
#include "JsonSerializationVisitor_Should.hpp"

namespace ContentMetadataApi_Tests
{ 
    TEST_P(JsonSerializationVisitorContentDto_Should, SerializeContentDto)
    {
        auto [uuid_str, title, description, duration, owner_id, video_file_id, image_file_id, upload_time_str] = GetParam();

        ContentMetadataApi::Dto::ContentDto content_dto;
        content_dto.m_uuid = boost::uuids::string_generator()(uuid_str);
        content_dto.m_title = title;
        content_dto.m_description = description;
        content_dto.m_duration = duration;
        content_dto.m_owner_id = boost::uuids::string_generator()(owner_id);
        content_dto.m_video_file_id = boost::uuids::string_generator()(video_file_id);
        content_dto.m_image_file_id = boost::uuids::string_generator()(image_file_id);

        ContentMetadataApi::Dto::LicenseRulesDto license_rule;
        const auto& license_rules_id_str = std::string("44556677-8899-aabb-ccdd-eeff00334455");
        license_rule.m_uuid = boost::uuids::string_generator()(license_rules_id_str);
        license_rule.m_duration = ContentMetadataCore::Enums::LicenseDuration::OneDay;
        license_rule.m_type = ContentMetadataCore::Enums::LicenseType::Rent;
        license_rule.m_price = 50;
        content_dto.m_license_rules.push_back(license_rule);

        EXPECT_CALL(*m_mock_date_time_parser, getIso8601String())
            .WillOnce(::testing::Return(upload_time_str));

        m_sut->visit(content_dto);
        validateJsonDocument(m_json_output);

        rapidjson::Document doc;
        doc.Parse(m_json_output.c_str());

        ASSERT_TRUE(doc.HasMember("uuid"));
        ASSERT_TRUE(doc.HasMember("title"));
        ASSERT_TRUE(doc.HasMember("description"));
        ASSERT_TRUE(doc.HasMember("duration"));
        ASSERT_TRUE(doc.HasMember("owner_id"));
        ASSERT_TRUE(doc.HasMember("video_file_id"));
        ASSERT_TRUE(doc.HasMember("image_file_id"));
        ASSERT_TRUE(doc.HasMember("upload_time"));
        ASSERT_TRUE(doc.HasMember("license_rules"));

        EXPECT_EQ(doc["uuid"].GetString(), uuid_str);
        EXPECT_EQ(doc["title"].GetString(), title);
        EXPECT_EQ(doc["description"].GetString(), description);
        EXPECT_EQ(doc["duration"].GetInt(), duration);
        EXPECT_EQ(doc["owner_id"].GetString(), owner_id);
        EXPECT_EQ(doc["video_file_id"].GetString(), video_file_id);
        EXPECT_EQ(doc["image_file_id"].GetString(), image_file_id);
        EXPECT_EQ(doc["upload_time"].GetString(), upload_time_str);

        const auto& license_rules_json = doc["license_rules"];
        ASSERT_TRUE(license_rules_json.IsArray());
        ASSERT_EQ(license_rules_json.Size(), 1);

        const auto& license_rule_json = license_rules_json[0];
        ASSERT_TRUE(license_rule_json.HasMember("uuid"));
        ASSERT_TRUE(license_rule_json.HasMember("price"));
        ASSERT_TRUE(license_rule_json.HasMember("type"));
        ASSERT_TRUE(license_rule_json.HasMember("duration"));

        auto s = license_rule_json["uuid"].GetString();

        //Note: While comparing to raw c strings (const char*) EXPECT_STREQ needs to be used, because EXPECT_EQ compares pointer value in that case.
        //EXPECT_STREQ(license_rule_json["uuid"].GetString(), "44556677-8899-aabb-ccdd-eeff00334455");
        EXPECT_EQ(license_rule_json["uuid"].GetString(), license_rules_id_str);
        EXPECT_EQ(license_rule_json["price"].GetInt(), 50);
        EXPECT_EQ(license_rule_json["type"].GetInt(), static_cast<int>(ContentMetadataCore::Enums::LicenseType::Rent));
        EXPECT_EQ(license_rule_json["duration"].GetInt(), static_cast<int>(ContentMetadataCore::Enums::LicenseDuration::OneDay));
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentDtoTests,
        JsonSerializationVisitorContentDto_Should,
        ::testing::Values(
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Sample Title", "Sample Description", 120,
                "11223344-5566-7788-99aa-bbccddeeff01", "11223344-5566-7788-99aa-bbccddeeff02", "11223344-5566-7788-99aa-bbccddeeff03", "2023-09-25T10:15:30Z"),

            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", "Another Title", "Another Description", 60,
                "22334455-6677-8899-aabb-ccddeeff0012", "22334455-6677-8899-aabb-ccddeeff0013", "22334455-6677-8899-aabb-ccddeeff0014", "2023-09-24T08:45:00Z"),

            std::make_tuple("33445566-7788-99aa-bbcc-ddeeff002233", "No Description", "", 0,
                "33445566-7788-99aa-bbcc-ddeeff002234", "33445566-7788-99aa-bbcc-ddeeff002235", "33445566-7788-99aa-bbcc-ddeeff002236", "2023-09-23T17:00:00Z"),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "", "Description for Empty Title", 360,
                "44556677-8899-aabb-ccdd-eeff00334456", "44556677-8899-aabb-ccdd-eeff00334457", "44556677-8899-aabb-ccdd-eeff00334458", "2023-09-22T12:30:00Z"),

            std::make_tuple("55667788-99aa-bbcc-ddee-ff0066778899", std::string(512, 'T'), "Long Title", 240,
                "55667788-99aa-bbcc-ddee-ff0066778801", "55667788-99aa-bbcc-ddee-ff0066778802", "55667788-99aa-bbcc-ddee-ff0066778803", "2023-09-21T09:15:00Z"),

            std::make_tuple("66778899-aabb-ccdd-eeff-00778899aabb", "Minimal Upload Time", "Sample Description", 180,
                "66778899-aabb-ccdd-eeff-00778899aabc", "66778899-aabb-ccdd-eeff-00778899aabd", "66778899-aabb-ccdd-eeff-00778899aabe", "1970-01-01T00:00:00Z"),

            std::make_tuple("66778899-aabb-ccdd-eeff-00778899aabb", "Max Upload Time", "Description with Max Time", 300,
                "66778899-aabb-ccdd-eeff-00778899aabc", "66778899-aabb-ccdd-eeff-00778899aabd", "66778899-aabb-ccdd-eeff-00778899aabe", "9999-12-31T23:59:59Z"),

            std::make_tuple("66778899-aabb-ccdd-eeff-00778899aabb", "Max Upload Time", "Description with Max Time", 3600,
                "66778899-aabb-ccdd-eeff-00778899aabc", "66778899-aabb-ccdd-eeff-00778899aabd", "66778899-aabb-ccdd-eeff-00778899aabe", "9999-12-31T23:59:59Z")
        )
    );

    TEST_P(JsonSerializationVisitorContentCommentDto_Should, SerializeContentCommentDto)
    {
        auto [uuid_str, body, user_name, creation_time_str, content_id_str] = GetParam();

        ContentMetadataApi::Dto::ContentCommentDto comment_dto;
        comment_dto.m_uuid = boost::uuids::string_generator()(uuid_str);
        comment_dto.m_body = body;
        comment_dto.m_user_name = user_name;
        comment_dto.m_content_id = boost::uuids::string_generator()(content_id_str);

        EXPECT_CALL(*m_mock_date_time_parser, getIso8601String())
            .WillOnce(::testing::Return(creation_time_str));

        m_sut->visit(comment_dto);
        validateJsonDocument(m_json_output);

        rapidjson::Document doc;
        doc.Parse(m_json_output.c_str());

        ASSERT_TRUE(doc.HasMember("uuid"));
        ASSERT_TRUE(doc.HasMember("body"));
        ASSERT_TRUE(doc.HasMember("user_name"));
        ASSERT_TRUE(doc.HasMember("creation_time"));
        ASSERT_TRUE(doc.HasMember("content_id"));

        EXPECT_EQ(doc["uuid"].GetString(), uuid_str);
        EXPECT_EQ(doc["body"].GetString(), body);
        EXPECT_EQ(doc["user_name"].GetString(), user_name);
        EXPECT_EQ(doc["creation_time"].GetString(), creation_time_str);
        EXPECT_EQ(doc["content_id"].GetString(), content_id_str);
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentCommentDtoTests,
        JsonSerializationVisitorContentCommentDto_Should,
        ::testing::Values(
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Sample body", "SampleUser", "2023-09-25T10:15:30Z", "33445566-7788-99aa-bbcc-ddeeff001122"),
            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", "", "UserWithoutBody", "2023-09-24T08:45:00Z", "22334455-6677-8899-aabb-ccddeeff0033"),
            std::make_tuple("33445566-7788-99aa-bbcc-ddeeff002233", "BodyWithoutUserName", "", "2023-09-23T17:00:00Z", "44556677-8899-aabb-ccdd-eeff00445566"),
            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", "BodyWithEmptyContentId", "SampleUser2", "2023-09-22T12:30:00Z", "00000000-0000-0000-0000-000000000000"),
            std::make_tuple("55667788-99aa-bbcc-ddee-ff0066778899", "", "", "2023-09-21T09:15:00Z", "00000000-0000-0000-0000-000000000000"),
            std::make_tuple("66778899-aabb-ccdd-eeff-00778899aabb", std::string(1024, 'A'), "LongUserName", "2023-09-20T07:45:00Z", "66778899-aabb-ccdd-eeff-00778899aabb"),
            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", "MinimalTimeComment", "MinUser", "1970-01-01T00:00:00Z", "55667788-99aa-bbcc-ddee-ff0066778899"),
            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0033", "MaxTimeComment", "MaxUser", "9999-12-31T23:59:59Z", "33445566-7788-99aa-bbcc-ddeeff002233")
        )
    );


    TEST_P(JsonSerializationVisitorLicenseRulesDto_Should, SerializeLicenseRulesDto)
    {
        auto [uuid_str, price, type, duration, is_duration_present] = GetParam();

        ContentMetadataApi::Dto::LicenseRulesDto license_dto;
        license_dto.m_uuid = boost::uuids::string_generator()(uuid_str);
        license_dto.m_price = price;
        license_dto.m_type = type;

        if (is_duration_present)
        {
            license_dto.m_duration = duration;
        }

        m_sut->visit(license_dto);
        validateJsonDocument(m_json_output);

        rapidjson::Document doc;
        doc.Parse(m_json_output.c_str());

        ASSERT_TRUE(doc.HasMember("uuid"));
        ASSERT_TRUE(doc.HasMember("price"));
        ASSERT_TRUE(doc.HasMember("type"));

        EXPECT_EQ(doc["uuid"].GetString(), uuid_str);
        EXPECT_EQ(doc["price"].GetInt(), price);
        EXPECT_EQ(doc["type"].GetInt(), static_cast<int>(type));

        if (is_duration_present)
        {
            ASSERT_TRUE(doc.HasMember("duration"));
            EXPECT_EQ(doc["duration"].GetInt(), static_cast<int>(duration));
        }
        else
        {
            ASSERT_FALSE(doc.HasMember("duration"));
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        LicenseRulesDtoTests,
        JsonSerializationVisitorLicenseRulesDto_Should,
        ::testing::Values(
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", 100, ContentMetadataCore::Enums::LicenseType::Rent,
                ContentMetadataCore::Enums::LicenseDuration::OneDay, true),

            std::make_tuple("22334455-6677-8899-aabb-ccddeeff0011", 0, ContentMetadataCore::Enums::LicenseType::Buy,
                ContentMetadataCore::Enums::LicenseDuration::Unknown, false),

            std::make_tuple("33445566-7788-99aa-bbcc-ddeeff002233", -50, ContentMetadataCore::Enums::LicenseType::Buy,
                ContentMetadataCore::Enums::LicenseDuration::Unknown, false),

            std::make_tuple("44556677-8899-aabb-ccdd-eeff00334455", 250, ContentMetadataCore::Enums::LicenseType::Rent,
                ContentMetadataCore::Enums::LicenseDuration::Week, true),

            std::make_tuple("00000000-0000-0000-0000-000000000000", 500, ContentMetadataCore::Enums::LicenseType::Rent,
                ContentMetadataCore::Enums::LicenseDuration::Month, true),

            std::make_tuple("55667788-99aa-bbcc-ddee-ff0066778899", 1000, ContentMetadataCore::Enums::LicenseType::Unknown,
                ContentMetadataCore::Enums::LicenseDuration::Unknown, false)
        )
    );


    TEST_P(JsonSerializationVisitorErrorsDto_Should, SerializeErrorsDto)
    {
        auto [errors] = GetParam();

        ContentMetadataApi::Dto::ErrorsDto errors_dto;
        errors_dto.m_errors = errors;

        m_sut->visit(errors_dto);
        validateJsonDocument(m_json_output);

        rapidjson::Document doc;
        doc.Parse(m_json_output.c_str());

        ASSERT_TRUE(doc.HasMember("errors"));
        const auto& errors_array = doc["errors"];
        ASSERT_TRUE(errors_array.IsArray());
        EXPECT_EQ(errors_array.Size(), errors.size());

        for (size_t i = 0; i < errors.size(); ++i)
        {
            EXPECT_EQ(errors_array[i].GetString(), errors[i]);
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        ErrorsDtoTests,
        JsonSerializationVisitorErrorsDto_Should,
        ::testing::Values(
            std::make_tuple(std::vector<std::string>{"Error1", "Error2"}),
            std::make_tuple(std::vector<std::string>{})
        )
    );

    TEST_P(JsonSerializationVisitorGuidDto_Should, SerializeGuidDto)
    {
        auto [uuid_str] = GetParam();

        ContentMetadataApi::Dto::GuidDto guid_dto;
        guid_dto.m_uuid = boost::uuids::string_generator()(uuid_str);

        m_sut->visit(guid_dto);
        validateJsonDocument(m_json_output);

        rapidjson::Document doc;
        doc.Parse(m_json_output.c_str());

        ASSERT_TRUE(doc.HasMember("uuid"));
        EXPECT_EQ(doc["uuid"].GetString(), uuid_str);
    }

    INSTANTIATE_TEST_SUITE_P(
        GuidDtoTests,
        JsonSerializationVisitorGuidDto_Should,
        ::testing::Values(
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00"),
            std::make_tuple("00000000-0000-0000-0000-000000000000")
        )
    );

    TEST_P(JsonSerializationVisitorContentsDto_Should, SerializeContentsDto)
    {
        auto [uuid_str1, title1, description1, duration1, owner_id1, video_file_id1, image_file_id1, upload_time_str1,
            uuid_str2, title2, description2, duration2, owner_id2, video_file_id2, image_file_id2, upload_time_str2] = GetParam();

        ContentMetadataApi::Dto::ContentDto content_dto1;
        content_dto1.m_uuid = boost::uuids::string_generator()(uuid_str1);
        content_dto1.m_title = title1;
        content_dto1.m_description = description1;
        content_dto1.m_duration = duration1;
        content_dto1.m_owner_id = boost::uuids::string_generator()(owner_id1);
        content_dto1.m_video_file_id = boost::uuids::string_generator()(video_file_id1);
        content_dto1.m_image_file_id = boost::uuids::string_generator()(image_file_id1);

        ContentMetadataApi::Dto::ContentDto content_dto2;
        content_dto2.m_uuid = boost::uuids::string_generator()(uuid_str2);
        content_dto2.m_title = title2;
        content_dto2.m_description = description2;
        content_dto2.m_duration = duration2;
        content_dto2.m_owner_id = boost::uuids::string_generator()(owner_id2);
        content_dto2.m_video_file_id = boost::uuids::string_generator()(video_file_id2);
        content_dto2.m_image_file_id = boost::uuids::string_generator()(image_file_id2);

        EXPECT_CALL(*m_mock_date_time_parser, getIso8601String())
            .WillOnce(::testing::Return(upload_time_str1))
            .WillOnce(::testing::Return(upload_time_str2));            

        ContentMetadataApi::Dto::ContentsDto contents_dto;
        contents_dto.m_contents.push_back(content_dto1);
        contents_dto.m_contents.push_back(content_dto2);

        m_sut->visit(contents_dto);
        validateJsonDocument(m_json_output);

        rapidjson::Document doc;
        doc.Parse(m_json_output.c_str());

        ASSERT_TRUE(doc.HasMember("contents"));
        const auto& contents_array = doc["contents"];
        ASSERT_TRUE(contents_array.IsArray());
        ASSERT_EQ(contents_array.Size(), 2);

        const auto& content1_json = contents_array[0];
        ASSERT_TRUE(content1_json.HasMember("uuid"));
        ASSERT_TRUE(content1_json.HasMember("title"));
        ASSERT_TRUE(content1_json.HasMember("description"));
        ASSERT_TRUE(content1_json.HasMember("duration"));
        ASSERT_TRUE(content1_json.HasMember("owner_id"));
        ASSERT_TRUE(content1_json.HasMember("video_file_id"));
        ASSERT_TRUE(content1_json.HasMember("image_file_id"));
        ASSERT_TRUE(content1_json.HasMember("upload_time"));

        EXPECT_EQ(content1_json["uuid"].GetString(), uuid_str1);
        EXPECT_EQ(content1_json["title"].GetString(), title1);
        EXPECT_EQ(content1_json["description"].GetString(), description1);
        EXPECT_EQ(content1_json["duration"].GetInt(), duration1);
        EXPECT_EQ(content1_json["owner_id"].GetString(), owner_id1);
        EXPECT_EQ(content1_json["video_file_id"].GetString(), video_file_id1);
        EXPECT_EQ(content1_json["image_file_id"].GetString(), image_file_id1);
        EXPECT_EQ(content1_json["upload_time"].GetString(), upload_time_str1);

        const auto& content2_json = contents_array[1];
        ASSERT_TRUE(content2_json.HasMember("uuid"));
        ASSERT_TRUE(content2_json.HasMember("title"));
        ASSERT_TRUE(content2_json.HasMember("description"));
        ASSERT_TRUE(content2_json.HasMember("duration"));
        ASSERT_TRUE(content2_json.HasMember("owner_id"));
        ASSERT_TRUE(content2_json.HasMember("video_file_id"));
        ASSERT_TRUE(content2_json.HasMember("image_file_id"));
        ASSERT_TRUE(content2_json.HasMember("upload_time"));

        EXPECT_EQ(content2_json["uuid"].GetString(), uuid_str2);
        EXPECT_EQ(content2_json["title"].GetString(), title2);
        EXPECT_EQ(content2_json["description"].GetString(), description2);
        EXPECT_EQ(content2_json["duration"].GetInt(), duration2);
        EXPECT_EQ(content2_json["owner_id"].GetString(), owner_id2);
        EXPECT_EQ(content2_json["video_file_id"].GetString(), video_file_id2);
        EXPECT_EQ(content2_json["image_file_id"].GetString(), image_file_id2);
        EXPECT_EQ(content2_json["upload_time"].GetString(), upload_time_str2);
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentsDtoTests,
        JsonSerializationVisitorContentsDto_Should,
        ::testing::Values(
            std::make_tuple("11223344-5566-7788-99aa-bbccddeeff00", "Title1", "Description1", 120,
                "11223344-5566-7788-99aa-bbccddeeff01", "11223344-5566-7788-99aa-bbccddeeff02", "11223344-5566-7788-99aa-bbccddeeff03", "2023-09-25T10:15:30Z",
                "22334455-6677-8899-aabb-ccddeeff0011", "Title2", "Description2", 60,
                "22334455-6677-8899-aabb-ccddeeff0012", "22334455-6677-8899-aabb-ccddeeff0013", "22334455-6677-8899-aabb-ccddeeff0014", "2023-09-24T08:45:00Z"),

            std::make_tuple("33445566-7788-99aa-bbcc-ddeeff002233", "No Description", "", 0,
                "33445566-7788-99aa-bbcc-ddeeff002234", "33445566-7788-99aa-bbcc-ddeeff002235", "33445566-7788-99aa-bbcc-ddeeff002236", "2023-09-23T17:00:00Z",
                "44556677-8899-aabb-ccdd-eeff00334455", "", "Another Description", 360,
                "44556677-8899-aabb-ccdd-eeff00334456", "44556677-8899-aabb-ccdd-eeff00334457", "44556677-8899-aabb-ccdd-eeff00334458", "2023-09-22T12:30:00Z"),

            std::make_tuple("55667788-99aa-bbcc-ddee-ff0066778899", std::string(512, 'T'), "Long Title", 240,
                "55667788-99aa-bbcc-ddee-ff0066778801", "55667788-99aa-bbcc-ddee-ff0066778802", "55667788-99aa-bbcc-ddee-ff0066778803", "2023-09-21T09:15:00Z",
                "66778899-aabb-ccdd-eeff-00778899aabb", "Minimal Upload Time", "Sample Description", 180,
                "66778899-aabb-ccdd-eeff-00778899aabc", "66778899-aabb-ccdd-eeff-00778899aabd", "66778899-aabb-ccdd-eeff-00778899aabe", "1970-01-01T00:00:00Z")
        )
    );

}