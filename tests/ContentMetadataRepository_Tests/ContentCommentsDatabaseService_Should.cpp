#include "ContentCommentsDatabaseService_Should.hpp"

namespace ContentMetadataRepository_Tests
{
    TEST_F(ContentCommentsDatabaseService_Should, AddContentCommentAsync_CommitsTransactionOnSuccess)
    {

        ContentMetadataCore::Entities::ContentComment comment;
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        boost::uuids::uuid mock_content_id = boost::uuids::random_generator()();
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();


        EXPECT_CALL(*m_unit_of_work_mock, contentCommentRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>
                {
                    co_return std::move(content_comment_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::An<const boost::uuids::uuid&>())).Times(0);

        EXPECT_CALL(*content_comment_repo_mock, addAsync(::testing::An<ContentMetadataCore::Entities::ContentComment&>()))
            .WillOnce(::testing::Invoke([&](ContentMetadataCore::Entities::ContentComment&) -> boost::asio::awaitable<boost::uuids::uuid>
                {
                    co_return mock_content_id;
                }));

        boost::uuids::uuid result_uuid;

        EXPECT_NO_THROW({
            auto future = boost::asio::co_spawn(
                boost::asio::system_executor(),
                m_sut->addContentCommentAsync(comment),
                boost::asio::use_future
            );

            result_uuid = future.get();
            });

        EXPECT_EQ(result_uuid, mock_content_id);
    }

    TEST_F(ContentCommentsDatabaseService_Should, AddContentCommentAsync_RollsBackTransactionOnException)
    {
        ContentMetadataCore::Entities::ContentComment comment;
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentCommentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>
                {
                    co_return std::move(content_comment_repo_mock);
                }));

        EXPECT_CALL(*content_comment_repo_mock, addAsync(::testing::_))
            .WillOnce(::testing::Throw(std::runtime_error("Error adding comment")));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_));
        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_)).Times(0);

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->addContentCommentAsync(comment),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }

    TEST_F(ContentCommentsDatabaseService_Should, DeleteContentCommentAsync_CommitsTransactionOnSuccess)
    {
        boost::uuids::uuid comment_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentCommentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>
                {
                    co_return std::move(content_comment_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_)).Times(0);

        EXPECT_CALL(*content_comment_repo_mock, deleteAsync(comment_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->deleteContentCommentAsync(comment_id),
            boost::asio::use_future
        );

        EXPECT_NO_THROW(future.get());
    }

    TEST_F(ContentCommentsDatabaseService_Should, DeleteContentCommentAsync_RollsBackTransactionOnException)
    {
        boost::uuids::uuid comment_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentCommentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>
                {
                    co_return std::move(content_comment_repo_mock);
                }));

        EXPECT_CALL(*content_comment_repo_mock, deleteAsync(comment_id))
            .WillOnce(::testing::Throw(std::runtime_error("Error deleting comment")));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_));
        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_)).Times(0);

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->deleteContentCommentAsync(comment_id),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }

}
