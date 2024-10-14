#pragma once
#include "IContentDataMapper.hpp"
#include "IContentCommentDataMapper.hpp"
#include "ILicenseRulesDataMapper.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{
		class CONTENT_METADATA_API_EXPORT ContentDataMapper : public IContentDataMapper
		{
		public:
			explicit ContentDataMapper(
				std::shared_ptr<IContentCommentDataMapper> a_content_comment_mapper,
				std::shared_ptr<ILicenseRulesDataMapper> a_license_rules_mapper) noexcept;

			ContentDataMapper(const ContentDataMapper&) = delete;
			ContentDataMapper(ContentDataMapper&&) = delete;

			ContentDataMapper& operator=(const ContentDataMapper&) = delete;
			ContentDataMapper& operator=(ContentDataMapper&&) = delete;

			auto contentEntityFrom(const Dto::ContentDto& a_content_dto) const -> ContentMetadataCore::Entities::Content override;
			auto contentDtoFrom(const ContentMetadataCore::Entities::Content& a_content) const -> Dto::ContentDto override;

		private:
			std::shared_ptr<IContentCommentDataMapper>  m_content_comment_mapper;
			std::shared_ptr<ILicenseRulesDataMapper>	m_license_rules_mapper;
		};
	}
}