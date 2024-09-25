#pragma once
#include <gmock/gmock.h>
#include "../../ContentMetadataApi/Parsers/IDateTimeParser.hpp"
#include "../../ContentMetadataApi/Parsers/DateTimeParser.hpp"

namespace ContentMetadataApi_Tests 
{
    namespace Mocks 
    {
        class DateTimeParserMock : public ContentMetadataApi::Parsers::IDateTimeParser 
        {
        public:
            MOCK_METHOD(void, parseIso8601, (const std::string& a_date_time), (override));
            MOCK_METHOD(void, setTimePoint, (std::chrono::system_clock::time_point a_time_point), (noexcept, override));
            MOCK_METHOD(std::chrono::system_clock::time_point, getTimePoint, (), (const, noexcept, override));
            MOCK_METHOD(std::time_t, getTimeT, (), (const, noexcept, override));
            MOCK_METHOD(std::string, getIso8601String, (), (const, noexcept, override));

            std::unique_ptr<ContentMetadataApi::Parsers::DateTimeParser> real_date_time_parser;

            DateTimeParserMock()
            {
                real_date_time_parser = std::make_unique<ContentMetadataApi::Parsers::DateTimeParser>();
            }

            void delegateToReal()
            {
                ON_CALL(*this, parseIso8601(testing::An<const std::string&>()))
                    .WillByDefault([this](const std::string& a_date_time)
                        {
                            return real_date_time_parser->parseIso8601(a_date_time);
                        });

                ON_CALL(*this, setTimePoint(testing::An<std::chrono::system_clock::time_point>()))
                    .WillByDefault([this](std::chrono::system_clock::time_point a_date_time)
                        {
                            return real_date_time_parser->setTimePoint(a_date_time);
                        });

                ON_CALL(*this, getTimePoint())
                    .WillByDefault([this]()
                        {
                            return real_date_time_parser->getTimePoint();
                        });

                ON_CALL(*this, getTimeT())
                    .WillByDefault([this]()
                        {
                            return real_date_time_parser->getTimeT();
                        });

                ON_CALL(*this, getIso8601String())
                    .WillByDefault([this]()
                        {
                            return real_date_time_parser->getIso8601String();
                        });
            }
        };
    }
}
