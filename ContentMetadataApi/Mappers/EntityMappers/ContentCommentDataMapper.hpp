#pragma once
#include "IContentCommentDataMapper.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{
		class CONTENT_METADATA_API_EXPORT ContentCommentDataMapper : public IContentCommentDataMapper
		{
		public:
			ContentCommentDataMapper() = default;
			ContentCommentDataMapper(const ContentCommentDataMapper&) = delete;
			ContentCommentDataMapper(ContentCommentDataMapper&&) = delete;

			ContentCommentDataMapper& operator=(const ContentCommentDataMapper&) = delete;
			ContentCommentDataMapper& operator=(ContentCommentDataMapper&&) = delete;

			auto contentCommentEntityFrom(const Dto::ContentCommentDto& a_content_comment_dto) const -> ContentMetadataCore::Entities::ContentComment override;
			auto contentCommentDtoFrom(const ContentMetadataCore::Entities::ContentComment& a_content_comment_entity) const -> Dto::ContentCommentDto override;
		};
	}
}