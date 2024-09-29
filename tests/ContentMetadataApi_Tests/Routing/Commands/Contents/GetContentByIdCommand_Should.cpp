#include "GetContentByIdCommand_Should.hpp"
#include "../../../../../ContentMetadataApi/HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(GetContentByIdCommand_Should, ReturnOkResponseWhenContentIsRetrievedSuccessfully)
    {
        ContentMetadataCore::Entities::Content mock_content_entity;
        ContentMetadataApi::Dto::ContentDto mock_content_dto;

        EXPECT_CALL(*m_mock_contract, getContentMetadataByIdAsync(m_content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content> {
            co_return mock_content_entity;
                }));

        EXPECT_CALL(*m_mock_mapper, contentDtoFrom(::testing::An<const ContentMetadataCore::Entities::Content&>()))
            .WillOnce(::testing::Return(mock_content_dto));

        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::Ok);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "application/json");
    }


    TEST_F(GetContentByIdCommand_Should, ThrowRuntimeErrorWhenValidationFails)
    {
        std::vector<std::string> validation_errors = { "Validation Error 1", "Validation Error 2" };
        ContentMetadataCore::Entities::Content mock_content_entity;
        ContentMetadataApi::Dto::ContentDto mock_content_dto;

        EXPECT_CALL(*m_mock_contract, getContentMetadataByIdAsync(m_content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content> {
            co_return mock_content_entity;
                }));

        EXPECT_CALL(*m_mock_mapper, contentDtoFrom(::testing::_))
            .WillOnce(::testing::Return(mock_content_dto));

        EXPECT_CALL(*m_mock_visitor_factory, createDataValidationVisitor(::testing::_))
            .WillOnce([&validation_errors](std::vector<std::string>& errors) {
            errors = validation_errors;
            return std::make_unique<::testing::NiceMock<Mocks::VisitorMock>>();
                });

        EXPECT_THROW(
            {
                auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
                future.get();
            },
            std::runtime_error
        );
    }


    TEST_F(GetContentByIdCommand_Should, ThrowExceptionWhenContentRetrievalFails)
    {
        EXPECT_CALL(*m_mock_contract, getContentMetadataByIdAsync(m_content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content> {
            throw std::runtime_error("Content retrieval failed");
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
