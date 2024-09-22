#pragma once
#include "../StdAfx.hpp"

namespace ContentMetadataApi
{
	namespace Dto 
	{
		struct ContentDto;
		struct ContentCommentDto;
		struct LicenseRulesDto;
		struct GuidDto;
		struct ContentsDto;
		struct ErrorsDto;
	}

	namespace Visitors
	{
		class IVisitor
		{
		public:
			virtual ~IVisitor() = default;

			virtual void visit(Dto::ContentDto& a_visitable) = 0;
			virtual void visit(Dto::ContentCommentDto& a_visitable) = 0;
			virtual void visit(Dto::LicenseRulesDto& a_visitable) = 0;
			virtual void visit(Dto::GuidDto& a_visitable) = 0;
			virtual void visit(Dto::ContentsDto& a_visitable) = 0;
			virtual void visit(Dto::ErrorsDto& a_visitable) = 0; 
		};
	}
}