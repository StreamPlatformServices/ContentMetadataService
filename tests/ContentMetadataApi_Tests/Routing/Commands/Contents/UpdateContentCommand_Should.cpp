#include "UpdateContentCommand_Should.hpp"
#include "../../../../../ContentMetadataApi/HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(UpdateContentCommand_Should, ReturnOkResponseWhenContentUpdatedSuccessfully)
    {
        ContentMetadataCore::Entities::Content mock_content_entity;

        EXPECT_CALL(*m_mock_mapper, contentEntityFrom(::testing::_))
            .WillOnce(::testing::Return(mock_content_entity));

        EXPECT_CALL(*m_mock_contract, editContentMetadataAsync(m_content_id, ::testing::_))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid, ContentMetadataCore::Entities::Content) -> boost::asio::awaitable<void> 
                {
                    co_return;
                }));

        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::Ok);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "text/plain");
        EXPECT_EQ(response.m_body, "Content updated successfully");
    }

    TEST_F(UpdateContentCommand_Should, ReturnBadRequestWhenValidationFails)
    {
        std::vector<std::string> validation_errors = { "Validation Error 1", "Validation Error 2" };

        EXPECT_CALL(*m_mock_visitor_factory, createDataValidationVisitor(::testing::_))
            .WillOnce([&validation_errors](std::vector<std::string>& errors) {
            errors = validation_errors;
            return std::make_unique<::testing::NiceMock<Mocks::VisitorMock>>();
                });

        EXPECT_CALL(*m_mock_serialization_visitor, visit(testing::An<ContentMetadataApi::Dto::ErrorsDto&>()));

        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::BadRequest);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "application/json");
    }

    TEST_F(UpdateContentCommand_Should, ThrowExceptionWhenContentUpdateFails)
    {
        ContentMetadataCore::Entities::Content mock_content_entity;

        EXPECT_CALL(*m_mock_mapper, contentEntityFrom(::testing::_))
            .WillOnce(::testing::Return(mock_content_entity));

        EXPECT_CALL(*m_mock_contract, editContentMetadataAsync(m_content_id, ::testing::_))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid, ContentMetadataCore::Entities::Content) -> boost::asio::awaitable<void> {
                    throw std::runtime_error("Update failed");
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
