#include "DeleteContentCommand_Should.hpp"
#include "../../../../../ContentMetadataApi/HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(DeleteContentCommand_Should, ReturnOkResponseWhenContentDeletedSuccessfully)
    {
        EXPECT_CALL(*m_mock_contract, deleteContentMetadataAsync(m_content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<void> {
            co_return;
                }));

        auto future = boost::asio::co_spawn(boost::asio::system_executor{}, m_sut->executeAsync(), boost::asio::use_future);
        auto response = future.get();

        EXPECT_EQ(response.m_status_code, ContentMetadataApi::HttpStatusCode::Ok);
        EXPECT_EQ(response.m_headers[ContentMetadataApi::HeaderKeys::CONTENT_TYPE], "text/plain");
        EXPECT_EQ(response.m_body, "Content removed successfully.");
    }

    TEST_F(DeleteContentCommand_Should, ThrowExceptionWhenContentDeletionFails)
    {
        EXPECT_CALL(*m_mock_contract, deleteContentMetadataAsync(m_content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<void> {
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
