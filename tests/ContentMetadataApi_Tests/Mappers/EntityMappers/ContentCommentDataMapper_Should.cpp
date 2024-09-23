#pragma once
#include "ContentCommentDataMapper_Should.hpp"

TEST_P(ContentCommentDataMapper_Should, ConvertDtoToEntityCorrectly) {
    auto [uuid_str, body, hours_int, content_id_str, user_name] = GetParam();

    ContentMetadataApi::Dto::ContentCommentDto dto;
    dto.m_uuid = boost::uuids::string_generator()(uuid_str);
    dto.m_body = body;
    dto.m_creation_time = std::chrono::system_clock::now() - std::chrono::hours(hours_int);
    dto.m_content_id = boost::uuids::string_generator()(content_id_str);
    dto.m_user_name = user_name;

    ContentMetadataCore::Entities::ContentComment expected_entity;
    expected_entity.m_uuid = boost::uuids::string_generator()(uuid_str);
    expected_entity.m_body = body;
    expected_entity.m_creation_time = std::chrono::system_clock::now() - std::chrono::hours(hours_int);
    expected_entity.m_content_id = boost::uuids::string_generator()(content_id_str);
    expected_entity.m_user_name = user_name;

    auto actual_entity = m_sut->contentCommentEntityFrom(dto);

    EXPECT_EQ(boost::uuids::to_string(expected_entity.m_uuid), boost::uuids::to_string(actual_entity.m_uuid));
    EXPECT_EQ(expected_entity.m_body, actual_entity.m_body);
    EXPECT_TRUE(compareTimePoints(expected_entity.m_creation_time, actual_entity.m_creation_time));
    EXPECT_EQ(boost::uuids::to_string(expected_entity.m_content_id), boost::uuids::to_string(actual_entity.m_content_id));
    EXPECT_EQ(expected_entity.m_user_name, actual_entity.m_user_name);
}


TEST_P(ContentCommentDataMapper_Should, ConvertEntityToDtoCorrectly) {
    auto [uuid_str, body, hours_int, content_id_str, user_name] = GetParam();

    ContentMetadataCore::Entities::ContentComment entity;
    entity.m_uuid = boost::uuids::string_generator()(uuid_str);
    entity.m_body = body;
    entity.m_creation_time = std::chrono::system_clock::now() - std::chrono::hours(hours_int);
    entity.m_content_id = boost::uuids::string_generator()(content_id_str);
    entity.m_user_name = user_name;

    ContentMetadataApi::Dto::ContentCommentDto expected_dto;
    expected_dto.m_uuid = boost::uuids::string_generator()(uuid_str);
    expected_dto.m_body = body;
    expected_dto.m_creation_time = std::chrono::system_clock::now() - std::chrono::hours(hours_int);
    expected_dto.m_content_id = boost::uuids::string_generator()(content_id_str);
    expected_dto.m_user_name = user_name;

    auto actual_dto = m_sut->contentCommentDtoFrom(entity);

    EXPECT_EQ(boost::uuids::to_string(expected_dto.m_uuid), boost::uuids::to_string(actual_dto.m_uuid));
    EXPECT_EQ(expected_dto.m_body, actual_dto.m_body);
    EXPECT_TRUE(compareTimePoints(expected_dto.m_creation_time, actual_dto.m_creation_time));
    EXPECT_EQ(boost::uuids::to_string(expected_dto.m_content_id), boost::uuids::to_string(actual_dto.m_content_id));
    EXPECT_EQ(expected_dto.m_user_name, actual_dto.m_user_name);
}



INSTANTIATE_TEST_SUITE_P(
    ContentCommentMapperTests,
    ContentCommentDataMapper_Should,
    ::testing::Values(

        std::make_tuple(
            "00000000-0000-0000-0000-000000000000",  // UUID
            "body1",                                 // Body content
            0,                                       // 0 hours ago
            "00000000-0000-0000-0000-000000000000",  // Content ID
            "user1"),                                // User name

        std::make_tuple(
            "01234567-89ab-cdef-0123-456789abcdef",  // UUID 
            "Comment about API functionality",       // Body content
            48,                                      // 48 hours ago
            "11223344-5566-7788-99aa-bbccddeeff00",  // Content ID 
            "developer1"),                           // User name

        std::make_tuple(
            "22334455-6677-8899-aabb-ccddeeff0011",  // UUID 
            "",                                      // Body content 
            24,                                      // 24 hours ago
            "aabbccdd-eeff-0011-2233-445566778899",  // Content ID 
            "admin@company.com"),                    // User name

        std::make_tuple(
            "33445566-7788-99aa-bbcc-ddeeff002233",  // UUID 
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",  // Body content
            24 * 7,                                  // 7 days ago
            "33445566-7788-99aa-bbcc-ddeeff002233",  // Content ID 
            "content_creator"),                      // User name

        std::make_tuple(
            "44556677-8899-aabb-ccdd-eeff00334455",  // UUID 
            "Another insightful comment.",           // Body content
            24,                                      // 1 day ago
            "55446677-8899-aabb-ccdd-eeff00334455",  // Content ID
            "user123"),                              // User name

        std::make_tuple(
            "55667788-99aa-bbcc-ddee-ff0044556677",  // UUID 
            "This is a legacy comment from a retired system.",  // Body content
            8760,                                    // 1 year ago (8760 hours)
            "66778899-aabb-ccdd-eeff-005566778899",  // Content ID 
            "legacy_user"),                          // User name

        std::make_tuple(
            "66778899-aabb-ccdd-eeff-005566778899",  // UUID 
            "Data from system migration.",           // Body content
            2,                                       // 2 hours ago
            "778899aa-bbcc-ddee-eeff-ff0066778899",  // Content ID 
            "12345678"),                             // User name

        std::make_tuple(
            "778899aa-bbcc-ddee-bbcc-ff0066778899",  // UUID 
            "Anonymous comment.",                    // Body content
            0,                                       // 0 hours ago
            "8899aabb-ccdd-eeff-bbcc-00778899aabb",  // Content ID 
            "")                                      // User name
    )
);




