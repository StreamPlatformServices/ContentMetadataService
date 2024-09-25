#pragma once
#include <gtest/gtest.h>
#include "../../../ContentMetadataApi/Parsers/IDateTimeParser.hpp"
#include "../../../ContentMetadataApi/Parsers/DateTimeParser.hpp"


namespace ContentMetadataApi_Tests
{

    class DateTimeParser_Should : public ::testing::Test 
    {
    protected:
        std::unique_ptr<ContentMetadataApi::Parsers::IDateTimeParser> m_sut = std::make_unique<ContentMetadataApi::Parsers::DateTimeParser>();

        std::string toIso8601(std::time_t time) 
        {
            std::stringstream ss;
            ss << std::put_time(std::gmtime(&time), "%FT%TZ");
            return ss.str();
        }
    };

}