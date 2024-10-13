#pragma once
#include "ContentCommentRepository_Should.hpp"

namespace ContentMetadataRepository_Tests
{
    TEST_F(ContentCommentRepository_Should, AddContentComment)
    {
        ContentMetadataCore::Entities::ContentComment comment;
        comment.m_uuid = boost::uuids::random_generator()();
        comment.m_body = "Sample Comment";
        comment.m_creation_time = std::chrono::system_clock::now();
        comment.m_content_id = m_content_id;
        comment.m_user_name = "test_user";

        EXPECT_CALL(*m_date_time_parser_mock, setTimePoint(comment.m_creation_time)).Times(1);

        ON_CALL(*m_date_time_parser_mock, getPgSqlDatabaseFormat())
            .WillByDefault(::testing::Return(formatPgSqlTimestamp(comment.m_creation_time)));

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->addAsync(comment),
            boost::asio::use_future
        );

        m_thread_pool.join();

        boost::uuids::uuid result = future.get();
        EXPECT_EQ(result, comment.m_uuid);

        m_transaction->commit();

        pqxx::work transaction(*m_connection);
        std::string sql = std::format(
            "SELECT * FROM {} WHERE {} = '{}'",
            ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
            ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
            boost::uuids::to_string(comment.m_uuid)
        );
        auto res = transaction.exec(sql);
        EXPECT_EQ(res.size(), 1);
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME].as<std::string>(), boost::uuids::to_string(comment.m_uuid));
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::BODY_COLUMN_NAME].as<std::string>(), comment.m_body);
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::CREATION_TIME_COLUMN_NAME].as<std::string>(), formatPgSqlTimestamp(comment.m_creation_time));
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::CONTENT_UUID_COLUMN_NAME].as<std::string>(), boost::uuids::to_string(comment.m_content_id));
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::USER_NAME_COLUMN_NAME].as<std::string>(), comment.m_user_name);

        transaction.abort();
    }


    TEST_F(ContentCommentRepository_Should, DeleteContentComment)
    {
        auto comment_id = boost::uuids::random_generator()();
        insertComment(comment_id);

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->deleteAsync(comment_id),
            boost::asio::use_future
        );

        m_thread_pool.join();
        future.get();

        m_transaction->commit();

        pqxx::work transaction_check(*m_connection);
        auto sql = std::format(
            "SELECT * FROM {} WHERE {} = '{}'",
            ContentMetadataRepository::DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
            ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
            boost::uuids::to_string(comment_id)
        );
        auto res = transaction_check.exec(sql);
        EXPECT_EQ(res.size(), 0);

        transaction_check.abort();
    }


    TEST_F(ContentCommentRepository_Should, GetCommentsByContentId)
    {
        auto comments_ids = std::vector<boost::uuids::uuid>
        {
            boost::uuids::random_generator()(),
            boost::uuids::random_generator()(),
            boost::uuids::random_generator()()
        };
        auto creation_time = std::chrono::system_clock::now();
        insertComments(creation_time, comments_ids);

        EXPECT_CALL(*m_date_time_parser_mock, parsePgSqlFormat(formatPgSqlTimestamp(creation_time))).Times(3);

        ON_CALL(*m_date_time_parser_mock, getTimePoint())
            .WillByDefault(::testing::Return(creation_time));

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->getByContentIdAsync(m_content_id),
            boost::asio::use_future
        );

        m_thread_pool.join();

        m_transaction->commit();

        auto comments = future.get();

        EXPECT_EQ(comments.size(), 3);

        int i = 1;
        for (const auto& comment : comments) {
            EXPECT_EQ(comment.m_content_id, m_content_id);
            EXPECT_EQ(comment.m_body, std::format("Sample Comment {}", i));
            EXPECT_EQ(formatPgSqlTimestamp(comment.m_creation_time), formatPgSqlTimestamp(creation_time));
            EXPECT_EQ(comment.m_user_name, std::format("test_user{}", i));
            EXPECT_EQ(comment.m_uuid, comments_ids[i - 1]);
            ++i;
        }
    }
}
