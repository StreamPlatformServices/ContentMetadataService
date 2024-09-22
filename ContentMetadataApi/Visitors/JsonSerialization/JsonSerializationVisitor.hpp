#pragma once
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "../IVisitor.hpp"
#include "../../DataTransferObjects/ContentDto.hpp"
#include "../../Parsers/IDateTimeParser.hpp"
#include "../../DataTransferObjects/ContentCommentDto.hpp"
#include "../../DataTransferObjects/LicenseRulesDto.hpp"
#include "../../DataTransferObjects/GuidDto.hpp"
#include "../../DataTransferObjects/ErrorsDto.hpp"
#include "../../DataTransferObjects/ContentsDto.hpp"

namespace ContentMetadataApi
{
	namespace Visitors
	{
		class JsonSerializationVisitor : public IVisitor
		{
		public:
			explicit JsonSerializationVisitor(
				std::string& a_json_string,
				std::shared_ptr<Parsers::IDateTimeParser> a_date_time_parser) noexcept;

			JsonSerializationVisitor(const JsonSerializationVisitor&) = delete;
			JsonSerializationVisitor(JsonSerializationVisitor&&) = delete;

			JsonSerializationVisitor& operator=(const JsonSerializationVisitor&) = delete;
			JsonSerializationVisitor& operator=(JsonSerializationVisitor&&) = delete;

			void visit(Dto::ContentDto& a_visitable) override;
			void visit(Dto::ContentCommentDto& a_visitable) override;
			void visit(Dto::LicenseRulesDto& a_visitable) override;
			void visit(Dto::GuidDto& a_visitable) override;
			void visit(Dto::ErrorsDto& a_visitable) override;
			void visit(Dto::ContentsDto& a_visitable) override;

		private:
			rapidjson::Value serializeFrom(Dto::ContentDto& a_content_dto, rapidjson::Document::AllocatorType& a_allocator) const;
			rapidjson::Value serializeFrom(Dto::ContentCommentDto& a_content_comment_dto, rapidjson::Document::AllocatorType& a_allocator) const;
			rapidjson::Value serializeFrom(Dto::LicenseRulesDto& a_license_rules_dto, rapidjson::Document::AllocatorType& a_allocator) const;

		private:
			std::string& m_json_string;
			std::shared_ptr<Parsers::IDateTimeParser> m_date_time_parser;
		};
	}
}