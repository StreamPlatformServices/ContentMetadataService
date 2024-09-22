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

			virtual ContentMetadataCore::Entities::Content contentEntityFrom(const Dto::ContentDto& a_content_dto) const = 0;
			virtual Dto::ContentDto contentDtoFrom(const ContentMetadataCore::Entities::Content& a_content) const = 0;

		};
	}
}