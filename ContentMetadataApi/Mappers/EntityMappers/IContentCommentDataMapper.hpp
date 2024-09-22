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

			virtual ContentMetadataCore::Entities::ContentComment contentCommentEntityFrom(const Dto::ContentCommentDto& a_content_comment_dto) const = 0;
			virtual Dto::ContentCommentDto contentCommentDtoFrom(const ContentMetadataCore::Entities::ContentComment& a_content_comment_entity) const = 0;

		};
	}
}