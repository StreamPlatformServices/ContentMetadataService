#include "Entities/Content.hpp"
#include "ContentCommentsService.hpp"
#include "ContentsService.hpp"

namespace ContentMetadataCore 
{
    boost::asio::awaitable<boost::uuids::uuid> ContentCommentsService::addContentCommentAsync(Entities::ContentComment a_content_comment)
    {
        co_return boost::uuids::uuid{};
    }

    boost::asio::awaitable<void> ContentCommentsService::deleteContentCommentAsync(boost::uuids::uuid a_content_comment_id)
    {
        co_return;
    }


    boost::asio::awaitable<std::vector<Entities::Content>> ContentsService::getAllContentsAsync(int limit, int offset)
    {
        std::vector<Entities::Content> entities;

        for (int i = 0; i < limit; ++i)
        {
            Entities::Content entity;
            entity.m_uuid = boost::uuids::random_generator()();
            entity.m_title = "Random Title " + std::to_string(i + offset);
            entity.m_upload_time = std::chrono::system_clock::now() - std::chrono::hours(i * 24); 
            entity.m_duration = 300 + (i * 60); 
            entity.m_description = "This is a random description for content " + std::to_string(i + offset);

            Entities::LicenseRules license_rules;
            license_rules.m_duration = Enums::LicenseDuration::OneDay;
            license_rules.m_type = Enums::LicenseType::Rent;
            entity.m_license_rules.push_back(license_rules);
            entity.m_content_comments = {};

            entity.m_owner_id = boost::uuids::random_generator()();
            entity.m_video_file_id = boost::uuids::random_generator()();
            entity.m_image_file_id = boost::uuids::random_generator()();

            entities.push_back(std::move(entity));
        }

        co_return entities;
    }

    boost::asio::awaitable<Entities::Content> ContentsService::getContentMetadataByIdAsync(boost::uuids::uuid contentId)
    {
        co_return Entities::Content{};
    }

    boost::asio::awaitable<std::vector<Entities::Content>> ContentsService::getContentMetadataByOwnerIdAsync(boost::uuids::uuid ownerId)
    {
        co_return std::vector<Entities::Content>{};
    }

    boost::asio::awaitable<void> ContentsService::editContentMetadataAsync(boost::uuids::uuid contentId, Entities::Content content)
    {
        co_return;
    }

    boost::asio::awaitable<boost::uuids::uuid> ContentsService::addContentMetadataAsync(Entities::Content content)
    {
        co_return boost::uuids::uuid{};
    }

    boost::asio::awaitable<void> ContentsService::deleteContentMetadataAsync(boost::uuids::uuid contentId)
    {
        co_return;
    }
} // namespace ContentMetadataCore
