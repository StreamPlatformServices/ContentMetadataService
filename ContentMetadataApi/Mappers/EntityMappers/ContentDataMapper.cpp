#pragma once
#include "ContentDataMapper.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{        
        ContentDataMapper::ContentDataMapper(
            std::shared_ptr<IContentCommentDataMapper> a_content_comment_mapper,
            std::shared_ptr<ILicenseRulesDataMapper> a_license_rules_mapper) noexcept
            : m_content_comment_mapper(a_content_comment_mapper)
            , m_license_rules_mapper(a_license_rules_mapper)
        {
        }

        auto ContentDataMapper::contentEntityFrom(const Dto::ContentDto& a_content_dto) const -> ContentMetadataCore::Entities::Content
        {
            ContentMetadataCore::Entities::Content content_entity;

            content_entity.m_uuid = a_content_dto.m_uuid;
            content_entity.m_title = a_content_dto.m_title;
            content_entity.m_upload_time = a_content_dto.m_upload_time;
            content_entity.m_duration = a_content_dto.m_duration;
            content_entity.m_description = a_content_dto.m_description;

            for (auto& license_rules_dto : a_content_dto.m_license_rules)
            {
                content_entity.m_license_rules.push_back(m_license_rules_mapper->licenseRulesEntityFrom(license_rules_dto));
            }

            if (a_content_dto.m_content_comments)
            {
                for (auto& content_comments_dto : *a_content_dto.m_content_comments)
                {
                    content_entity.m_content_comments.push_back(m_content_comment_mapper->contentCommentEntityFrom(content_comments_dto));
                }
            }

            content_entity.m_owner_id = a_content_dto.m_owner_id;
            content_entity.m_video_file_id = a_content_dto.m_video_file_id;
            content_entity.m_image_file_id = a_content_dto.m_image_file_id;

            return content_entity;
        }

        auto ContentDataMapper::contentDtoFrom(const ContentMetadataCore::Entities::Content& a_content_entity) const -> Dto::ContentDto
        {
            Dto::ContentDto content_dto;

            content_dto.m_uuid = a_content_entity.m_uuid;
            content_dto.m_title = a_content_entity.m_title;
            content_dto.m_upload_time = a_content_entity.m_upload_time;
            content_dto.m_duration = a_content_entity.m_duration;
            content_dto.m_description = a_content_entity.m_description;

            for (auto& license_rules_entity : a_content_entity.m_license_rules)
            {
                content_dto.m_license_rules.push_back(m_license_rules_mapper->licenseRulesDtoFrom(license_rules_entity));
            }

            if (a_content_entity.m_content_comments.size() > 0)
            {
                content_dto.m_content_comments = std::vector<Dto::ContentCommentDto>();
                for (auto& content_comments_entity : a_content_entity.m_content_comments)
                {
                    content_dto.m_content_comments->push_back(m_content_comment_mapper->contentCommentDtoFrom(content_comments_entity));
                }
            }

            content_dto.m_owner_id = a_content_entity.m_owner_id;
            content_dto.m_video_file_id = a_content_entity.m_video_file_id;
            content_dto.m_image_file_id = a_content_entity.m_image_file_id;

            return content_dto;
        }
	}
}