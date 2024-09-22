#pragma once
#include "../IVisitor.hpp"
#include "../../Parsers/IDateTimeParser.hpp"
#include "../../Parsers/IGuidParser.hpp"
#include "../../DataTransferObjects/ContentDto.hpp"
#include "../../DataTransferObjects/ContentCommentDto.hpp"
#include "../../DataTransferObjects/LicenseRulesDto.hpp"
#include "../../DataTransferObjects/GuidDto.hpp"
#include "../../DataTransferObjects/ErrorsDto.hpp"
#include "../../DataTransferObjects/ContentsDto.hpp"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>


namespace ContentMetadataApi
{
	namespace Visitors
	{
		class JsonDeserializationVisitor : public IVisitor
		{

		public:
			explicit JsonDeserializationVisitor(
				const std::string& a_json_string,
				std::shared_ptr<Parsers::IDateTimeParser> a_date_time_parser,
				std::shared_ptr<Parsers::IGuidParser> a_guid_parser) noexcept;

			JsonDeserializationVisitor(const JsonDeserializationVisitor&) = delete;
			JsonDeserializationVisitor(JsonDeserializationVisitor&&) = delete;

			JsonDeserializationVisitor& operator=(const JsonDeserializationVisitor&) = delete;
			JsonDeserializationVisitor& operator=(JsonDeserializationVisitor&&) = delete;

			void visit(Dto::ContentDto& a_visitable) override;
			void visit(Dto::ContentCommentDto& a_visitable) override;
			void visit(Dto::LicenseRulesDto& a_visitable) override;
			void visit(Dto::GuidDto& a_visitable) override;
			void visit(Dto::ErrorsDto& a_visitable) override;
			void visit(Dto::ContentsDto& a_visitable) override;

		private:
			Dto::ContentDto deserializeContent(const rapidjson::Value& a_content_json_value) const;
			Dto::ContentCommentDto deserializeContentComment(const rapidjson::Value& a_content_comment_json_value) const;
			Dto::LicenseRulesDto deserializeLicenseRules(const rapidjson::Value& a_license_rules_json_value) const;

			bool isLicenseTypeEnumInRange(const int a_enum_int_value) const noexcept;
			bool isLicenseDurationEnumInRange(const int a_enum_int_value) const noexcept;

		private:

			std::string m_json_string;
			std::shared_ptr<Parsers::IDateTimeParser> m_date_time_parser;
			std::shared_ptr<Parsers::IGuidParser> m_guid_parser;
		};
	}
}