#include "AddContentCommentCommand_Should.hpp"
#include "../../../../../ContentMetadataApi/DataTransferObjects/ErrorsDto.hpp"
#include "../../../../../ContentMetadataApi/HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(AddContentCommentCommand_Should, ReturnOkResponseWhenCommentAddedSuccessfully)
    {
        boost::uuids::uuid generated_uuid = boost::uuids::random_generator()();
        ContentMetadataCore::Entities::ContentComment mock_comment_entity;

        EXPECT_CALL(*m_mock_mapper, contentCommentEntityFrom(::testing::_))
            .WillOnce(::testing::Return(mock_comment_entity));

        EXPECT_CALL(*m_mock_contract, addContentCommentAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](ContentMetadataCore::Entities::ContentComment) -> boost::asio::awaitable<boost::uuids::uuid> 
                {
                    co_return generated_uuid;
                }));

        EXPECT_CALL(*m_mock_deserialization_visitor, visit(testing::An<ContentMetadataApi::Dto::ContentCommentDto&>())).Times(1);
        EXPECT_CALL(*m_mock_validation_visitor, visit(testing::An<ContentMetadataApi::Dto::ContentCommentDto&>())).Times(1);
        EXPECT_CALL(*m_mock_serialization_visitor, visit(testing::An<ContentMetadataApi::Dto::GuidDto&>())).Times(1);

        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::Ok);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "application/json");
    }

    TEST_F(AddContentCommentCommand_Should, ReturnBadRequestWhenValidationFails)
    {
        std::vector<std::string> validation_errors = { "Validation Error 1", "Validation Error 2" };

        EXPECT_CALL(*m_mock_visitor_factory, createDataValidationVisitor(::testing::_))
            .WillOnce([&validation_errors](std::vector<std::string>& errors) 
                {
                    errors = validation_errors;
                    return std::make_unique<::testing::NiceMock<Mocks::VisitorMock>>();
                });

        EXPECT_CALL(*m_mock_visitor_factory, createJsonSerializationVisitor(::testing::_))
            .WillOnce([this](std::string& body) 
                {
                    return std::move(m_mock_serialization_visitor);
                });

        EXPECT_CALL(*m_mock_serialization_visitor, visit(::testing::An<ContentMetadataApi::Dto::ErrorsDto&>())).Times(1);

        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::BadRequest);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "application/json");
    }


    TEST_F(AddContentCommentCommand_Should, ThrowExceptionWhenCommentAddFails)
    {
        EXPECT_CALL(*m_mock_contract, addContentCommentAsync(::testing::_))
            .WillOnce(::testing::Invoke([](ContentMetadataCore::Entities::ContentComment) -> boost::asio::awaitable<boost::uuids::uuid> 
                {
                    throw std::runtime_error("Add comment failed");
                }));

        EXPECT_THROW(
            {
                auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
                future.get();
            },
            std::runtime_error
        );
    }
}
