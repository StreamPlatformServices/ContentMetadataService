#include "DeleteContentCommentCommand_Should.hpp"
#include "../../../../../ContentMetadataApi/HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(DeleteContentCommentCommand_Should, ReturnOkResponseWhenCommentDeletedSuccessfully)
    {
        EXPECT_CALL(*m_mock_contract, deleteContentCommentAsync(m_content_comment_id))
            .WillOnce(::testing::Invoke([](boost::uuids::uuid) -> boost::asio::awaitable<void> 
                {
                    co_return;
                }));

        
        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::Ok);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "text/plain");
        EXPECT_EQ(response.m_body, "Content comment removed successfully.");
    }

    TEST_F(DeleteContentCommentCommand_Should, ThrowExceptionWhenDeleteCommentFails)
    {
        EXPECT_CALL(*m_mock_contract, deleteContentCommentAsync(m_content_comment_id))
            .WillOnce(::testing::Invoke([](boost::uuids::uuid) -> boost::asio::awaitable<void> {
            throw std::runtime_error("Delete failed");
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
