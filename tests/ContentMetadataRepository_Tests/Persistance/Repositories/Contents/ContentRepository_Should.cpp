#include "ContentRepository_Should.hpp"

namespace ContentMetadataRepository_Tests
{
    TEST_F(ContentRepository_Should, AddContent)
    {
        ContentMetadataCore::Entities::Content content;
        content.m_uuid = boost::uuids::random_generator()();
        content.m_title = "New Content";
        content.m_upload_time = std::chrono::system_clock::now();
        content.m_duration = 120;
        content.m_description = "This is a test description";
        content.m_owner_id = boost::uuids::random_generator()();
        content.m_video_file_id = boost::uuids::random_generator()();
        content.m_image_file_id = boost::uuids::random_generator()();

        EXPECT_CALL(*m_date_time_parser_mock, setTimePoint(content.m_upload_time)).Times(1);

        ON_CALL(*m_date_time_parser_mock, getPgSqlDatabaseFormat())
            .WillByDefault(::testing::Return(formatPgSqlTimestamp(content.m_upload_time)));

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->addAsync(content),
            boost::asio::use_future
        );

        boost::uuids::uuid result = future.get();

        m_thread_pool.join();

        m_transaction->commit();


        EXPECT_EQ(result, content.m_uuid);

        pqxx::work transaction_check(*m_connection);
        auto sql = std::format(
            "SELECT * FROM {} WHERE {} = '{}'",
            ContentMetadataRepository::DatabaseKeys::CONTENT_TABLE_NAME,
            ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
            boost::uuids::to_string(content.m_uuid)
        );
        auto res = transaction_check.exec(sql);
        EXPECT_EQ(res.size(), 1);
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::TITLE_COLUMN_NAME].as<std::string>(), content.m_title);
        EXPECT_EQ(res[0][ContentMetadataRepository::DatabaseKeys::DESCRIPTION_COLUMN_NAME].as<std::string>(), content.m_description);

        transaction_check.abort();
    }

    TEST_F(ContentRepository_Should, GetContentById)
    {
        insertContent(m_content_id, boost::uuids::random_generator()());

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->getByIdAsync(m_content_id),
            boost::asio::use_future
        );

        m_thread_pool.join();

        auto content = future.get();

        m_transaction->commit();

        EXPECT_EQ(content.m_uuid, m_content_id);
        EXPECT_EQ(content.m_title, "Sample Content");
    }

    TEST_F(ContentRepository_Should, DeleteContent)
    {
        insertContent(m_content_id, boost::uuids::random_generator()());

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->deleteAsync(m_content_id),
            boost::asio::use_future
        );

        m_thread_pool.join();
        future.get();

        m_transaction->commit();

        pqxx::work transaction_check(*m_connection);
        auto sql = std::format(
            "SELECT * FROM {} WHERE {} = '{}'",
            ContentMetadataRepository::DatabaseKeys::CONTENT_TABLE_NAME,
            ContentMetadataRepository::DatabaseKeys::UUID_COLUMN_NAME,
            boost::uuids::to_string(m_content_id)
        );
        auto res = transaction_check.exec(sql);
        EXPECT_EQ(res.size(), 0);

        transaction_check.abort();
    }

    TEST_F(ContentRepository_Should, GetContentByOwnerId)
    {
        auto owner_id = boost::uuids::random_generator()();
        insertContent(m_content_id, owner_id);
        insertContent(boost::uuids::random_generator()(), boost::uuids::random_generator()());

        auto future = boost::asio::co_spawn(
            m_thread_pool.get_executor(),
            m_sut->getByOwnerIdAsync(owner_id),
            boost::asio::use_future
        );

        auto contents = future.get();

        m_thread_pool.join();

        m_transaction->commit();

        EXPECT_EQ(contents.size(), 1);
        EXPECT_EQ(contents[0].m_owner_id, owner_id);
    }
}
