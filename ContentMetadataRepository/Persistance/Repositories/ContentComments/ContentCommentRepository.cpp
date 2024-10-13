#pragma once
#include "ContentCommentRepository.hpp"
#include "../../../Constants/DatabaseKeys.hpp"

namespace ContentMetadataRepository
{
    ContentCommentRepository::ContentCommentRepository(
        std::shared_ptr<pqxx::work> a_transaction,
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager,
        std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser) noexcept
        : m_transaction(a_transaction)
        , m_thread_pool_manager(a_thread_pool_manager)
        , m_date_time_parser(a_date_time_parser)
    {
    }

    auto ContentCommentRepository::addAsync(ContentMetadataCore::Entities::ContentComment& a_content_comment) -> boost::asio::awaitable<boost::uuids::uuid> 
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            if (a_content_comment.m_uuid.is_nil())
            {
                a_content_comment.m_uuid = boost::uuids::random_generator()();
            }

            m_date_time_parser->setTimePoint(a_content_comment.m_creation_time);

            std::string sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}) VALUES ('{}', '{}', '{}', '{}', '{}');",
                DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                DatabaseKeys::UUID_COLUMN_NAME, DatabaseKeys::BODY_COLUMN_NAME, DatabaseKeys::CREATION_TIME_COLUMN_NAME, DatabaseKeys::CONTENT_UUID_COLUMN_NAME, DatabaseKeys::USER_NAME_COLUMN_NAME,
                boost::uuids::to_string(a_content_comment.m_uuid),
                m_transaction->esc(a_content_comment.m_body),
                m_date_time_parser->getPgSqlDatabaseFormat(),
                boost::uuids::to_string(a_content_comment.m_content_id),
                m_transaction->esc(a_content_comment.m_user_name)
            );

            m_transaction->exec0(sql);

            co_return a_content_comment.m_uuid;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error adding comment: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentCommentRepository::deleteAsync(boost::uuids::uuid a_content_comment_id) -> boost::asio::awaitable<void>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "DELETE FROM {} WHERE {} = '{}';",
                DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                DatabaseKeys::UUID_COLUMN_NAME,
                boost::uuids::to_string(a_content_comment_id)
            );

            m_transaction->exec0(sql);

            co_return;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error deleting comment: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentCommentRepository::getByContentIdAsync(boost::uuids::uuid a_content_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::ContentComment>>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "SELECT * FROM {} WHERE {} = '{}';",
                DatabaseKeys::CONTENT_COMMENT_TABLE_NAME,
                DatabaseKeys::CONTENT_UUID_COLUMN_NAME,
                boost::uuids::to_string(a_content_id)
            );
            pqxx::result result = m_transaction->exec(sql);

            std::vector<ContentMetadataCore::Entities::ContentComment> comments;
            for (const auto& row : result)
            {
                comments.push_back(mapRowToComment(row));
            }

            co_return comments;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error fetching comments by content id: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentCommentRepository::mapRowToComment(const pqxx::row& a_row) -> ContentMetadataCore::Entities::ContentComment
    {
        ContentMetadataCore::Entities::ContentComment comment;
        comment.m_uuid = boost::uuids::string_generator()(a_row[DatabaseKeys::UUID_COLUMN_NAME].as<std::string>());
        comment.m_body = a_row[DatabaseKeys::BODY_COLUMN_NAME].as<std::string>();

        m_date_time_parser->parsePgSqlFormat(a_row[DatabaseKeys::CREATION_TIME_COLUMN_NAME].as<std::string>());
        comment.m_creation_time = m_date_time_parser->getTimePoint();

        comment.m_content_id = boost::uuids::string_generator()(a_row[DatabaseKeys::CONTENT_UUID_COLUMN_NAME].as<std::string>());
        comment.m_user_name = a_row[DatabaseKeys::USER_NAME_COLUMN_NAME].as<std::string>();

        return comment;
    }
}
