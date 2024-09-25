#include "ContentDataMapper_Should.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_P(ContentDataMapper_Should, ConvertDtoToEntityCorrectly) {
        auto [uuid_str, title, upload_time_int, duration, description, owner_id_str, video_file_id_str, image_file_id_str] = GetParam();

        ContentMetadataApi::Dto::ContentDto dto;
        dto.m_uuid = boost::uuids::string_generator()(uuid_str);
        dto.m_title = title;
        dto.m_upload_time = std::chrono::system_clock::now() - std::chrono::hours(upload_time_int);
        dto.m_duration = duration;
        dto.m_description = description;
        dto.m_owner_id = boost::uuids::string_generator()(owner_id_str);
        dto.m_video_file_id = boost::uuids::string_generator()(video_file_id_str);
        dto.m_image_file_id = boost::uuids::string_generator()(image_file_id_str);

        ContentMetadataCore::Entities::Content expected_entity;
        expected_entity.m_uuid = boost::uuids::string_generator()(uuid_str);
        expected_entity.m_title = title;
        expected_entity.m_upload_time = std::chrono::system_clock::now() - std::chrono::hours(upload_time_int);
        expected_entity.m_duration = duration;
        expected_entity.m_description = description;
        expected_entity.m_owner_id = boost::uuids::string_generator()(owner_id_str);
        expected_entity.m_video_file_id = boost::uuids::string_generator()(video_file_id_str);
        expected_entity.m_image_file_id = boost::uuids::string_generator()(image_file_id_str);

        auto actual_entity = m_sut->contentEntityFrom(dto);

        EXPECT_EQ(boost::uuids::to_string(expected_entity.m_uuid), boost::uuids::to_string(actual_entity.m_uuid));
        EXPECT_EQ(expected_entity.m_title, actual_entity.m_title);
        EXPECT_EQ(expected_entity.m_duration, actual_entity.m_duration);
        EXPECT_EQ(expected_entity.m_description, actual_entity.m_description);
        EXPECT_EQ(boost::uuids::to_string(expected_entity.m_owner_id), boost::uuids::to_string(actual_entity.m_owner_id));
        EXPECT_EQ(boost::uuids::to_string(expected_entity.m_video_file_id), boost::uuids::to_string(actual_entity.m_video_file_id));
        EXPECT_EQ(boost::uuids::to_string(expected_entity.m_image_file_id), boost::uuids::to_string(actual_entity.m_image_file_id));
    }

    TEST_P(ContentDataMapper_Should, ConvertEntityToDtoCorrectly) {
        auto [uuid_str, title, upload_time_int, duration, description, owner_id_str, video_file_id_str, image_file_id_str] = GetParam();

        ContentMetadataCore::Entities::Content entity;
        entity.m_uuid = boost::uuids::string_generator()(uuid_str);
        entity.m_title = title;
        entity.m_upload_time = std::chrono::system_clock::now() - std::chrono::hours(upload_time_int);
        entity.m_duration = duration;
        entity.m_description = description;
        entity.m_owner_id = boost::uuids::string_generator()(owner_id_str);
        entity.m_video_file_id = boost::uuids::string_generator()(video_file_id_str);
        entity.m_image_file_id = boost::uuids::string_generator()(image_file_id_str);

        ContentMetadataApi::Dto::ContentDto expected_dto;
        expected_dto.m_uuid = boost::uuids::string_generator()(uuid_str);
        expected_dto.m_title = title;
        expected_dto.m_upload_time = std::chrono::system_clock::now() - std::chrono::hours(upload_time_int);
        expected_dto.m_duration = duration;
        expected_dto.m_description = description;
        expected_dto.m_owner_id = boost::uuids::string_generator()(owner_id_str);
        expected_dto.m_video_file_id = boost::uuids::string_generator()(video_file_id_str);
        expected_dto.m_image_file_id = boost::uuids::string_generator()(image_file_id_str);

        auto actual_dto = m_sut->contentDtoFrom(entity);

        EXPECT_EQ(boost::uuids::to_string(expected_dto.m_uuid), boost::uuids::to_string(actual_dto.m_uuid));
        EXPECT_EQ(expected_dto.m_title, actual_dto.m_title);
        EXPECT_EQ(expected_dto.m_duration, actual_dto.m_duration);
        EXPECT_EQ(expected_dto.m_description, actual_dto.m_description);
        EXPECT_EQ(boost::uuids::to_string(expected_dto.m_owner_id), boost::uuids::to_string(actual_dto.m_owner_id));
        EXPECT_EQ(boost::uuids::to_string(expected_dto.m_video_file_id), boost::uuids::to_string(actual_dto.m_video_file_id));
        EXPECT_EQ(boost::uuids::to_string(expected_dto.m_image_file_id), boost::uuids::to_string(actual_dto.m_image_file_id));
    }

    INSTANTIATE_TEST_SUITE_P(
        ContentDataMapperTests,
        ContentDataMapper_Should,
        ::testing::Values(
            std::make_tuple(
                "00000000-0000-0000-0000-000000000000",  // UUID
                "Title 1",                              // Title
                0,                                      // Upload time (now)
                3600,                                   // Duration in seconds
                "Description 1",                        // Description
                "00000000-0000-0000-0000-000000000000",  // Owner UUID
                "00000000-0000-0000-0000-000000000000",  // Video file UUID
                "00000000-0000-0000-0000-000000000000"   // Image file UUID
            ),
            std::make_tuple(
                "11223344-5566-7788-99aa-bbccddeeff00",  // UUID
                "Title 2",                              // Title
                48,                                     // Upload time (48 hours ago)
                7200,                                   // Duration in seconds
                "Description 2",                        // Description
                "11223344-5566-7788-99aa-bbccddeeff00",  // Owner UUID
                "11223344-5566-7788-99aa-bbccddeeff00",  // Video file UUID
                "11223344-5566-7788-99aa-bbccddeeff00"   // Image file UUID
            ),
            std::make_tuple(
                "22334455-6677-8899-aabb-ccddeeff0011",  // UUID
                "Title 3",                              // Title
                24,                                     // Upload time (24 hours ago)
                1800,                                   // Duration in seconds
                "Description 3",                        // Description
                "22334455-6677-8899-aabb-ccddeeff0011",  // Owner UUID
                "22334455-6677-8899-aabb-ccddeeff0011",  // Video file UUID
                "22334455-6677-8899-aabb-ccddeeff0011"   // Image file UUID
            ),
            std::make_tuple(
                "33445566-7788-99aa-bbcc-ddeeff002233",  // UUID
                "Title 4",                              // Title
                7 * 24,                                 // Upload time (7 days ago)
                5400,                                   // Duration in seconds
                "Description 4",                        // Description
                "33445566-7788-99aa-bbcc-ddeeff002233",  // Owner UUID
                "33445566-7788-99aa-bbcc-ddeeff002233",  // Video file UUID
                "33445566-7788-99aa-bbcc-ddeeff002233"   // Image file UUID
            )
        )
    );
}