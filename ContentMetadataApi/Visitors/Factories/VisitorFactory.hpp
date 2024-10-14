#pragma once
#include "IVisitorFactory.hpp"
#include "../../../ContentMetadataCommon/DateTime/IDateTimeParser.hpp"
#include "../../Parsers/IGuidParser.hpp"

namespace ContentMetadataApi
{
	namespace Visitors
	{
		class CONTENT_METADATA_API_EXPORT VisitorFactory : public IVisitorFactory
		{
		public:
			explicit VisitorFactory(
				std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser,
				std::shared_ptr<Parsers::IGuidParser> a_guid_parser) noexcept;

			VisitorFactory(const VisitorFactory&) = delete;
			VisitorFactory(VisitorFactory&&) = delete;

			VisitorFactory& operator=(const VisitorFactory&) = delete;
			VisitorFactory& operator=(VisitorFactory&&) = delete;
			
			auto createDataValidationVisitor(std::vector<std::string>& a_errors_output) const -> std::unique_ptr<IVisitor> override;
			auto createJsonDeserializationVisitor(const std::string& a_json_string_input) const -> std::unique_ptr<IVisitor> override;
			auto createJsonSerializationVisitor(std::string& a_json_string_output) const -> std::unique_ptr<IVisitor> override;

		private:
			std::shared_ptr<ContentMetadataCommon::IDateTimeParser> m_date_time_parser;
			std::shared_ptr<Parsers::IGuidParser> m_guid_parser;
		};
	}
}