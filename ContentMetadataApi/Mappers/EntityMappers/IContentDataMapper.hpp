#pragma once
#include "../../StdAfx.hpp"
#include "../../DataTransferObjects/ContentDto.hpp"
#include "../../../ContentMetadataCore/Entities/Content.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{
		
		class IContentDataMapper
		{
		public:
			virtual ~IContentDataMapper() = default;

			virtual auto contentEntityFrom(const Dto::ContentDto& a_content_dto) const -> ContentMetadataCore::Entities::Content = 0;
			virtual auto contentDtoFrom(const ContentMetadataCore::Entities::Content& a_content) const ->Dto::ContentDto = 0;

		};
	}
}