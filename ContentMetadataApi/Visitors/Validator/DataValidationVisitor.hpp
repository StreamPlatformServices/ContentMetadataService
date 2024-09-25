#pragma once
#include "../IVisitor.hpp"
#include "../../Parsers/IGuidParser.hpp"
#include "../../DataTransferObjects/ContentDto.hpp"
#include "../../DataTransferObjects/ContentCommentDto.hpp"
#include "../../DataTransferObjects/LicenseRulesDto.hpp"
#include "../../DataTransferObjects/GuidDto.hpp"
#include "../../DataTransferObjects/ErrorsDto.hpp"
#include "../../DataTransferObjects/ContentsDto.hpp"


namespace ContentMetadataApi
{
	namespace Visitors
	{
		class CONTENT_METADATA_API_EXPORT DataValidationVisitor : public IVisitor
		{
		public:
			explicit DataValidationVisitor(
				std::vector<std::string>& errors, 
				std::shared_ptr<Parsers::IGuidParser> a_guid_parser) noexcept;

			DataValidationVisitor(const DataValidationVisitor&) = delete;
			DataValidationVisitor(DataValidationVisitor&&) = delete;

			DataValidationVisitor& operator=(const DataValidationVisitor&) = delete;
			DataValidationVisitor& operator=(DataValidationVisitor&&) = delete;

			void visit(Dto::ContentDto& a_visitable) override;
			void visit(Dto::ContentCommentDto& a_visitable) override;
			void visit(Dto::LicenseRulesDto& a_visitable) override;
			void visit(Dto::GuidDto& a_visitable) override;
			void visit(Dto::ErrorsDto& a_visitable) override;
			void visit(Dto::ContentsDto& a_visitable) override;

		private:
			std::vector<std::string>& m_errors;
			std::shared_ptr<Parsers::IGuidParser> m_guid_parser;
		};
	}
}