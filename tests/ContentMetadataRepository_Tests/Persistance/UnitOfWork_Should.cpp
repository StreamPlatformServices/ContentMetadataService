#include "UnitOfWork_Should.hpp"

namespace ContentMetadataRepository_Tests
{
    TEST_F(UnitOfWork_Should, ThrowExceptionWhenCommitNotExistingTransaction)
    {
        auto future = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->commitTransactionAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }

    TEST_F(UnitOfWork_Should, ThrowExceptionWhenRollbackNotExistingTransaction)
    {
        EXPECT_THROW(m_sut->rollbackTransaction(m_test_transaction_id), std::runtime_error);
    }

    TEST_F(UnitOfWork_Should, returnContentRepositoryAndCommitTransaction)
    {
        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        EXPECT_CALL(*m_repository_factory_mock, createContentRepository(::testing::An<std::shared_ptr<pqxx::work>>()))
            .WillOnce(::testing::Return(::testing::ByMove(std::move(content_repo_mock))));

        EXPECT_CALL(*m_thread_pool_manager_mock, getThreadPool())
            .WillRepeatedly(::testing::ReturnRef(m_thread_pool));

        auto future_repository = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->contentRepositoryAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        auto result = future_repository.get();
        ASSERT_TRUE(result != nullptr);

        auto future_commit = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->commitTransactionAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        EXPECT_NO_THROW(future_commit.get());
    }

    TEST_F(UnitOfWork_Should, returnContentRepositoryAndRollbackTransaction)
    {
        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        EXPECT_CALL(*m_repository_factory_mock, createContentRepository(::testing::An<std::shared_ptr<pqxx::work>>()))
            .WillOnce(::testing::Return(::testing::ByMove(std::move(content_repo_mock))));

        EXPECT_CALL(*m_thread_pool_manager_mock, getThreadPool())
            .WillRepeatedly(::testing::ReturnRef(m_thread_pool));

        auto future_repository = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->contentRepositoryAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        auto result = future_repository.get();
        ASSERT_TRUE(result != nullptr);

        EXPECT_NO_THROW(m_sut->rollbackTransaction(m_test_transaction_id));
    }

    TEST_F(UnitOfWork_Should, returnContentCommentRepositoryAndCommitTransaction)
    {
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();
        EXPECT_CALL(*m_repository_factory_mock, createContentCommentRepository(::testing::An<std::shared_ptr<pqxx::work>>()))
            .WillOnce(::testing::Return(::testing::ByMove(std::move(content_comment_repo_mock))));

        EXPECT_CALL(*m_thread_pool_manager_mock, getThreadPool())
            .WillRepeatedly(::testing::ReturnRef(m_thread_pool));

        auto future_repository = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->contentCommentRepositoryAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        auto result = future_repository.get();
        ASSERT_TRUE(result != nullptr);

        auto future_commit = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->commitTransactionAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        EXPECT_NO_THROW(future_commit.get());
    }

    TEST_F(UnitOfWork_Should, returnContentCommentRepositoryAndRollbackTransaction)
    {
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();
        EXPECT_CALL(*m_repository_factory_mock, createContentCommentRepository(::testing::An<std::shared_ptr<pqxx::work>>()))
            .WillOnce(::testing::Return(::testing::ByMove(std::move(content_comment_repo_mock))));

        EXPECT_CALL(*m_thread_pool_manager_mock, getThreadPool())
            .WillRepeatedly(::testing::ReturnRef(m_thread_pool));

        auto future_repository = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->contentCommentRepositoryAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        auto result = future_repository.get();
        ASSERT_TRUE(result != nullptr);

        EXPECT_NO_THROW(m_sut->rollbackTransaction(m_test_transaction_id));
    }

    TEST_F(UnitOfWork_Should, returnLicenseRulesRepositoryAndCommitTransaction)
    {
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();
        EXPECT_CALL(*m_repository_factory_mock, createLicenseRulesRepository(::testing::An<std::shared_ptr<pqxx::work>>()))
            .WillOnce(::testing::Return(::testing::ByMove(std::move(license_rules_repo_mock))));

        EXPECT_CALL(*m_thread_pool_manager_mock, getThreadPool())
            .WillRepeatedly(::testing::ReturnRef(m_thread_pool));

        auto future_repository = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->licenseRulesRepositoryAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        auto result = future_repository.get();
        ASSERT_TRUE(result != nullptr);

        auto future_commit = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->commitTransactionAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        EXPECT_NO_THROW(future_commit.get());
    }

    TEST_F(UnitOfWork_Should, returnLicenseRulesRepositoryAndRollbackTransaction)
    {
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();
        EXPECT_CALL(*m_repository_factory_mock, createLicenseRulesRepository(::testing::An<std::shared_ptr<pqxx::work>>()))
            .WillOnce(::testing::Return(::testing::ByMove(std::move(license_rules_repo_mock))));

        EXPECT_CALL(*m_thread_pool_manager_mock, getThreadPool())
            .WillRepeatedly(::testing::ReturnRef(m_thread_pool));

        auto future_repository = boost::asio::co_spawn(
            m_thread_pool,
            m_sut->licenseRulesRepositoryAsync(m_test_transaction_id),
            boost::asio::use_future
        );

        auto result = future_repository.get();
        ASSERT_TRUE(result != nullptr);

        EXPECT_NO_THROW(m_sut->rollbackTransaction(m_test_transaction_id));
    }
} // namespace ContentMetadataRepository_Tests

