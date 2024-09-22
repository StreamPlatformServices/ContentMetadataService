#include "VisitorFactory.hpp"
#include "../Validator/DataValidationVisitor.hpp"
#include "../JsonSerialization/JsonDeserializationVisitor.hpp"
#include "../JsonSerialization/JsonSerializationVisitor.hpp"

namespace ContentMetadataApi
{
    namespace Visitors
    {
        VisitorFactory::VisitorFactory(
            std::shared_ptr<Parsers::IDateTimeParser> a_date_time_parser,
            std::shared_ptr<Parsers::IGuidParser> a_guid_parser) noexcept
            : m_date_time_parser(a_date_time_parser)
            , m_guid_parser(a_guid_parser)
        {
        }

        std::unique_ptr<IVisitor> VisitorFactory::createDataValidationVisitor(std::vector<std::string>& a_errors_output) const
        {
            return std::make_unique<DataValidationVisitor>(a_errors_output, m_guid_parser);
        }

        std::unique_ptr<IVisitor> VisitorFactory::createJsonDeserializationVisitor(const std::string& a_json_string_input) const
        {
            return std::make_unique<JsonDeserializationVisitor>(a_json_string_input, m_date_time_parser, m_guid_parser);
        }

        std::unique_ptr<IVisitor> VisitorFactory::createJsonSerializationVisitor(std::string& a_json_string_output) const
        {
            return std::make_unique<JsonSerializationVisitor>(a_json_string_output, m_date_time_parser);
        }

        
    }
}