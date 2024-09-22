#pragma once
#include "ContentCommentDataMapper.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{  

        ContentMetadataCore::Entities::ContentComment ContentCommentDataMapper::contentCommentEntityFrom(const Dto::ContentCommentDto& a_content_comment_dto) const
        {
            ContentMetadataCore::Entities::ContentComment content_comment_entity;

            content_comment_entity.m_uuid = a_content_comment_dto.m_uuid;
            content_comment_entity.m_body = a_content_comment_dto.m_body;
            content_comment_entity.m_creation_time = a_content_comment_dto.m_creation_time;
            content_comment_entity.m_content_id = a_content_comment_dto.m_content_id;
            content_comment_entity.m_user_name = a_content_comment_dto.m_user_name;

            return content_comment_entity;
        }

        Dto::ContentCommentDto ContentCommentDataMapper::contentCommentDtoFrom(const ContentMetadataCore::Entities::ContentComment& a_content_comment_entity) const
        {
            Dto::ContentCommentDto content_comment_dto;

            content_comment_dto.m_uuid = a_content_comment_entity.m_uuid;
            content_comment_dto.m_body = a_content_comment_entity.m_body;
            content_comment_dto.m_creation_time = a_content_comment_entity.m_creation_time;
            content_comment_dto.m_content_id = a_content_comment_entity.m_content_id;
            content_comment_dto.m_user_name = a_content_comment_entity.m_user_name;

            return content_comment_dto;
        }
	}
}