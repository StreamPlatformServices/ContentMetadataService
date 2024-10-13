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
			
			std::unique_ptr<IVisitor> createDataValidationVisitor(std::vector<std::string>& a_errors_output) const override;
			std::unique_ptr<IVisitor> createJsonDeserializationVisitor(const std::string& a_json_string_input) const override;
			std::unique_ptr<IVisitor> createJsonSerializationVisitor(std::string& a_json_string_output) const override;

		private:
			std::shared_ptr<ContentMetadataCommon::IDateTimeParser> m_date_time_parser;
			std::shared_ptr<Parsers::IGuidParser> m_guid_parser;
		};
	}
}