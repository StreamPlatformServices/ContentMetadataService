#include "ContentRepository.hpp"
#include "../../../Constants/DatabaseKeys.hpp"

namespace ContentMetadataRepository
{
    ContentRepository::ContentRepository(
        std::shared_ptr<pqxx::work> a_transaction,
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager,
        std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser) noexcept
        : m_transaction(a_transaction)
        , m_thread_pool_manager(a_thread_pool_manager)
        , m_date_time_parser(a_date_time_parser)
    {
    }

    auto ContentRepository::getAllAsync(int a_limit, int a_offset) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "SELECT * FROM {} LIMIT {} OFFSET {};",
                DatabaseKeys::CONTENT_TABLE_NAME,
                a_limit,
                a_offset
            );
            pqxx::result result = m_transaction->exec(sql);

            std::vector<ContentMetadataCore::Entities::Content> contents;
            for (const auto& row : result)
            {
                contents.push_back(mapRowToContent(row));
            }

            co_return contents;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error while getting contents: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentRepository::getByIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "SELECT * FROM {} WHERE {} = '{}';",
                DatabaseKeys::CONTENT_TABLE_NAME,
                DatabaseKeys::UUID_COLUMN_NAME,
                boost::uuids::to_string(a_content_id)
            );
            pqxx::result result = m_transaction->exec(sql);

            if (result.empty()) {
                throw std::runtime_error("Content not found");
            }

            co_return mapRowToContent(result[0]);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error while getting content by id: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentRepository::getByOwnerIdAsync(const boost::uuids::uuid& a_owner_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "SELECT * FROM {} WHERE {} = '{}';",
                DatabaseKeys::CONTENT_TABLE_NAME,
                DatabaseKeys::OWNER_ID_COLUMN_NAME,
                boost::uuids::to_string(a_owner_id)
            );
            pqxx::result result = m_transaction->exec(sql);

            std::vector<ContentMetadataCore::Entities::Content> contents;
            for (const auto& row : result)
            {
                contents.push_back(mapRowToContent(row));
            }

            co_return contents;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error while getting content by owner id: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentRepository::addAsync(ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<boost::uuids::uuid>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            if (a_content.m_uuid.is_nil())
            {
                a_content.m_uuid = boost::uuids::random_generator()();
            }

            m_date_time_parser->setTimePoint(a_content.m_upload_time);

            std::string sql = std::format(
                "INSERT INTO {} ({}, {}, {}, {}, {}, {}, {}, {}) VALUES ('{}', '{}', '{}', {}, '{}', '{}', '{}', '{}');",
                DatabaseKeys::CONTENT_TABLE_NAME,
                DatabaseKeys::UUID_COLUMN_NAME,
                DatabaseKeys::TITLE_COLUMN_NAME,
                DatabaseKeys::UPLOAD_TIME_COLUMN_NAME,
                DatabaseKeys::DURATION_COLUMN_NAME,
                DatabaseKeys::DESCRIPTION_COLUMN_NAME,
                DatabaseKeys::OWNER_ID_COLUMN_NAME,
                DatabaseKeys::VIDEO_FILE_ID_COLUMN_NAME,
                DatabaseKeys::IMAGE_FILE_ID_COLUMN_NAME,
                boost::uuids::to_string(a_content.m_uuid),
                m_transaction->esc(a_content.m_title),
                m_date_time_parser->getPgSqlDatabaseFormat(),
                a_content.m_duration,
                m_transaction->esc(a_content.m_description),
                boost::uuids::to_string(a_content.m_owner_id),
                boost::uuids::to_string(a_content.m_video_file_id),
                boost::uuids::to_string(a_content.m_image_file_id)
            );

            m_transaction->exec0(sql);

            co_return a_content.m_uuid;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error adding content: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentRepository::updateAsync(const ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<void>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            m_date_time_parser->setTimePoint(a_content.m_upload_time);

            std::string sql = std::format(
                "UPDATE {} SET {} = '{}', {} = '{}', {} = {}, {} = '{}', {} = '{}', "
                "{} = '{}', {} = '{}' WHERE {} = '{}';",
                DatabaseKeys::CONTENT_TABLE_NAME,
                DatabaseKeys::TITLE_COLUMN_NAME, m_transaction->esc(a_content.m_title),
                DatabaseKeys::UPLOAD_TIME_COLUMN_NAME, m_date_time_parser->getPgSqlDatabaseFormat(),
                DatabaseKeys::DURATION_COLUMN_NAME, a_content.m_duration,
                DatabaseKeys::DESCRIPTION_COLUMN_NAME, m_transaction->esc(a_content.m_description),
                DatabaseKeys::OWNER_ID_COLUMN_NAME, boost::uuids::to_string(a_content.m_owner_id),
                DatabaseKeys::VIDEO_FILE_ID_COLUMN_NAME, boost::uuids::to_string(a_content.m_video_file_id),
                DatabaseKeys::IMAGE_FILE_ID_COLUMN_NAME, boost::uuids::to_string(a_content.m_image_file_id),
                DatabaseKeys::UUID_COLUMN_NAME, boost::uuids::to_string(a_content.m_uuid)
            );

            m_transaction->exec0(sql);

            co_return;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error updating content: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentRepository::deleteAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<void>
    {
        auto& pool = m_thread_pool_manager->getThreadPool();

        try
        {
            co_await boost::asio::post(pool, boost::asio::use_awaitable);

            std::string sql = std::format(
                "DELETE FROM {} WHERE {} = '{}';",
                DatabaseKeys::CONTENT_TABLE_NAME,
                DatabaseKeys::UUID_COLUMN_NAME,
                boost::uuids::to_string(a_content_id)
            );
            m_transaction->exec0(sql);
            co_return;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error deleting content: " << e.what() << std::endl;
            throw;
        }
    }

    auto ContentRepository::mapRowToContent(const pqxx::row& a_row) -> ContentMetadataCore::Entities::Content
    {
        ContentMetadataCore::Entities::Content content;

        boost::uuids::string_generator gen;

        content.m_uuid = gen(a_row[DatabaseKeys::UUID_COLUMN_NAME].as<std::string>());
        content.m_title = a_row[DatabaseKeys::TITLE_COLUMN_NAME].as<std::string>();

        m_date_time_parser->parsePgSqlFormat(a_row[DatabaseKeys::UPLOAD_TIME_COLUMN_NAME].as<std::string>());
        content.m_upload_time = m_date_time_parser->getTimePoint();

        content.m_duration = a_row[DatabaseKeys::DURATION_COLUMN_NAME].as<int>();
        content.m_description = a_row[DatabaseKeys::DESCRIPTION_COLUMN_NAME].as<std::string>();
        content.m_owner_id = gen(a_row[DatabaseKeys::OWNER_ID_COLUMN_NAME].as<std::string>());
        content.m_video_file_id = gen(a_row[DatabaseKeys::VIDEO_FILE_ID_COLUMN_NAME].as<std::string>());
        content.m_image_file_id = gen(a_row[DatabaseKeys::IMAGE_FILE_ID_COLUMN_NAME].as<std::string>());

        return content;
    }
}
