#pragma once
#include "../../StdAfx.hpp"
#include "../../DataTransferObjects/ContentCommentDto.hpp"
#include "../../../ContentMetadataCore/Entities/ContentComment.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{
		
		class IContentCommentDataMapper
		{
		public:
			virtual ~IContentCommentDataMapper() = default;

			virtual auto contentCommentEntityFrom(const Dto::ContentCommentDto& a_content_comment_dto) const -> ContentMetadataCore::Entities::ContentComment = 0;
			virtual auto contentCommentDtoFrom(const ContentMetadataCore::Entities::ContentComment& a_content_comment_entity) const -> Dto::ContentCommentDto = 0;

		};
	}
}