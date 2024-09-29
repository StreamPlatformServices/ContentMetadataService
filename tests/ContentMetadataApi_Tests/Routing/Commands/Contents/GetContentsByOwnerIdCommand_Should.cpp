#include "GetContentsByOwnerIdCommand_Should.hpp"
#include "../../../../../ContentMetadataApi/HttpDataStructures/HeaderKeys.hpp"
#include "../../../../../ContentMetadataApi/DataTransferObjects/ContentsDto.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(GetContentsByOwnerIdCommand_Should, ReturnOkResponseWithContents)
    {
        std::vector<ContentMetadataCore::Entities::Content> mock_content_entities =
        {
            {
                boost::uuids::random_generator()(),    // m_uuid
                "Title 1",                            // m_title
                std::chrono::system_clock::now(),      // m_upload_time
                3600,                                 // m_duration
                "Description 1",                      // m_description
                {
                    {
                        boost::uuids::random_generator()(),
                        100,
                        ContentMetadataCore::Enums::LicenseType::Rent,
                        ContentMetadataCore::Enums::LicenseDuration::OneDay
                    },
                    {
                        boost::uuids::random_generator()(),
                        200,
                        ContentMetadataCore::Enums::LicenseType::Buy,
                        std::nullopt
                    }
                },                                   // m_license_rules
                {},                                   // m_content_comments (pusta lista)
                boost::uuids::random_generator()(),    // m_owner_id
                boost::uuids::random_generator()(),    // m_video_file_id
                boost::uuids::random_generator()()     // m_image_file_id
            },
            {
                boost::uuids::random_generator()(),    // m_uuid
                "Title 2",                            // m_title
                std::chrono::system_clock::now(),      // m_upload_time
                5400,                                 // m_duration
                "Description 2",                      // m_description
                {
                    {
                        boost::uuids::random_generator()(),
                        150,
                        ContentMetadataCore::Enums::LicenseType::Rent,
                        ContentMetadataCore::Enums::LicenseDuration::Month
                    }
                },                                   // m_license_rules
                {
                    {
                        boost::uuids::random_generator()(), // m_uuid
                        "Sample comment",                   // m_body
                        std::chrono::system_clock::now(),    // m_creation_time
                        boost::uuids::random_generator()(),  // m_content_id
                        "User 1"                             // m_user_name
                    }
                },                                   // m_content_comments
                boost::uuids::random_generator()(),    // m_owner_id
                boost::uuids::random_generator()(),    // m_video_file_id
                boost::uuids::random_generator()()     // m_image_file_id
            }
        };

        EXPECT_CALL(*m_mock_contract, getContentMetadataByOwnerIdAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> {
            co_return mock_content_entities;
                }));


        for (std::size_t i = 0; i < mock_content_entities.size(); ++i)
        {
            const auto& entity = mock_content_entities[i];

            ContentMetadataApi::Dto::ContentDto content_dto;
            content_dto.m_uuid = entity.m_uuid;
            content_dto.m_title = entity.m_title;
            content_dto.m_upload_time = entity.m_upload_time;
            content_dto.m_duration = entity.m_duration;
            content_dto.m_description = entity.m_description;

            std::transform(entity.m_license_rules.begin(), entity.m_license_rules.end(),
                std::back_inserter(content_dto.m_license_rules),
                [](const ContentMetadataCore::Entities::LicenseRules& rule) -> ContentMetadataApi::Dto::LicenseRulesDto
                {
                    ContentMetadataApi::Dto::LicenseRulesDto rule_dto;
                    rule_dto.m_uuid = rule.m_uuid;
                    rule_dto.m_price = rule.m_price;
                    rule_dto.m_type = rule.m_type;
                    rule_dto.m_duration = rule.m_duration;
                    return rule_dto;
                });

            content_dto.m_content_comments = std::vector<ContentMetadataApi::Dto::ContentCommentDto>{};

            std::transform(entity.m_content_comments.begin(), entity.m_content_comments.end(),
                std::back_inserter(*content_dto.m_content_comments),
                [](const ContentMetadataCore::Entities::ContentComment& comment) -> ContentMetadataApi::Dto::ContentCommentDto
                {
                    ContentMetadataApi::Dto::ContentCommentDto comment_dto;
                    comment_dto.m_uuid = comment.m_uuid;
                    comment_dto.m_body = comment.m_body;
                    comment_dto.m_creation_time = comment.m_creation_time;
                    comment_dto.m_user_name = comment.m_user_name;
                    comment_dto.m_content_id = comment.m_content_id;
                    return comment_dto;
                });

            content_dto.m_owner_id = entity.m_owner_id;
            content_dto.m_video_file_id = entity.m_video_file_id;
            content_dto.m_image_file_id = entity.m_image_file_id;

            EXPECT_CALL(*m_mock_mapper, contentDtoFrom(::testing::An<const ContentMetadataCore::Entities::Content&>()))
                .WillRepeatedly(::testing::Return(content_dto));


            EXPECT_CALL(*m_mock_serialization_visitor, visit(::testing::An<ContentMetadataApi::Dto::ContentsDto&>()))
                .WillRepeatedly([i, &mock_content_entities](ContentMetadataApi::Dto::ContentsDto& contents_dto)
                    {
                        ASSERT_EQ(contents_dto.m_contents.size(), mock_content_entities.size());

                        EXPECT_EQ(contents_dto.m_contents[i].m_uuid, mock_content_entities[i].m_uuid);
                        EXPECT_EQ(contents_dto.m_contents[i].m_title, mock_content_entities[i].m_title);
                        EXPECT_EQ(contents_dto.m_contents[i].m_description, mock_content_entities[i].m_description);
                        EXPECT_EQ(contents_dto.m_contents[i].m_upload_time, mock_content_entities[i].m_upload_time);
                        EXPECT_EQ(contents_dto.m_contents[i].m_duration, mock_content_entities[i].m_duration);
                        EXPECT_EQ(contents_dto.m_contents[i].m_owner_id, mock_content_entities[i].m_owner_id);
                        EXPECT_EQ(contents_dto.m_contents[i].m_video_file_id, mock_content_entities[i].m_video_file_id);
                        EXPECT_EQ(contents_dto.m_contents[i].m_image_file_id, mock_content_entities[i].m_image_file_id);

                        ASSERT_EQ(contents_dto.m_contents[i].m_license_rules.size(), mock_content_entities[i].m_license_rules.size());
                        for (std::size_t j = 0; j < contents_dto.m_contents[i].m_license_rules.size(); ++j) {
                            EXPECT_EQ(contents_dto.m_contents[i].m_license_rules[j].m_uuid, mock_content_entities[i].m_license_rules[j].m_uuid);
                            EXPECT_EQ(contents_dto.m_contents[i].m_license_rules[j].m_price, mock_content_entities[i].m_license_rules[j].m_price);
                            EXPECT_EQ(contents_dto.m_contents[i].m_license_rules[j].m_type, mock_content_entities[i].m_license_rules[j].m_type);
                            EXPECT_EQ(contents_dto.m_contents[i].m_license_rules[j].m_duration, mock_content_entities[i].m_license_rules[j].m_duration);
                        }

                        if (mock_content_entities[i].m_content_comments.size() > 0) {
                            ASSERT_EQ(contents_dto.m_contents[i].m_content_comments->size(), mock_content_entities[i].m_content_comments.size());
                            for (std::size_t k = 0; k < contents_dto.m_contents[i].m_content_comments->size(); ++k) {
                                EXPECT_EQ(contents_dto.m_contents[i].m_content_comments->at(k).m_uuid, mock_content_entities[i].m_content_comments[k].m_uuid);
                                EXPECT_EQ(contents_dto.m_contents[i].m_content_comments->at(k).m_body, mock_content_entities[i].m_content_comments[k].m_body);
                                EXPECT_EQ(contents_dto.m_contents[i].m_content_comments->at(k).m_creation_time, mock_content_entities[i].m_content_comments[k].m_creation_time);
                                EXPECT_EQ(contents_dto.m_contents[i].m_content_comments->at(k).m_user_name, mock_content_entities[i].m_content_comments[k].m_user_name);
                                EXPECT_EQ(contents_dto.m_contents[i].m_content_comments->at(k).m_content_id, mock_content_entities[i].m_content_comments[k].m_content_id);
                            }
                        }

                    });
        }

        EXPECT_CALL(*m_mock_serialization_visitor, visit(::testing::An<ContentMetadataApi::Dto::ContentsDto&>()));

        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::Ok);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "application/json");
    }

    TEST_F(GetContentsByOwnerIdCommand_Should, ThrowExceptionWhenFetchingContentsFails)
    {
        EXPECT_CALL(*m_mock_contract, getContentMetadataByOwnerIdAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> {
            throw std::runtime_error("Fetching failed");
            co_return std::vector<ContentMetadataCore::Entities::Content>{};
                }));

        EXPECT_THROW(
            {
                auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
                future.get();
            },
            std::runtime_error
        );
    }

    TEST_F(GetContentsByOwnerIdCommand_Should, ThrowExceptionWhenValidationFails)
    {
        std::vector<std::string> validation_errors = { "Validation Error 1", "Validation Error 2" };

        EXPECT_CALL(*m_mock_visitor_factory, createDataValidationVisitor(::testing::_))
            .WillOnce([&validation_errors](std::vector<std::string>& errors) {
            errors = validation_errors;
            return std::make_unique<::testing::NiceMock<ValidationVisitorMock>>();
                });

        std::vector<ContentMetadataCore::Entities::Content> mock_content_entities =
        {
            {
                boost::uuids::random_generator()(),    // m_uuid
                "Title 1",                            // m_title
                std::chrono::system_clock::now(),      // m_upload_time
                3600,                                 // m_duration
                "Description 1",                      // m_description
                {
                    {
                        boost::uuids::random_generator()(),
                        100,
                        ContentMetadataCore::Enums::LicenseType::Rent,
                        ContentMetadataCore::Enums::LicenseDuration::OneDay
                    }
                },                                    // m_license_rules
                {},                                    // m_content_comments (pusta lista)
                boost::uuids::random_generator()(),    // m_owner_id
                boost::uuids::random_generator()(),    // m_video_file_id
                boost::uuids::random_generator()()     // m_image_file_id
            }
        };

        EXPECT_CALL(*m_mock_contract, getContentMetadataByOwnerIdAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> {
            co_return mock_content_entities;
                }));

        for (const auto& entity : mock_content_entities)
        {
            ContentMetadataApi::Dto::ContentDto content_dto;
            content_dto.m_uuid = entity.m_uuid;
            content_dto.m_title = entity.m_title;
            content_dto.m_upload_time = entity.m_upload_time;
            content_dto.m_duration = entity.m_duration;
            content_dto.m_description = entity.m_description;

            EXPECT_CALL(*m_mock_mapper, contentDtoFrom(::testing::An<const ContentMetadataCore::Entities::Content&>()))
                .WillRepeatedly(::testing::Return(content_dto));
        }

        EXPECT_THROW(
            {
                auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
                future.get();
            },
            std::runtime_error);
    }

}
