#pragma once
#include <gtest/gtest.h>
#include "../../../ContentMetadataCommon/DateTime/IDateTimeParser.hpp"
#include "../../../ContentMetadataCommon/DateTime/DateTimeParser.hpp"


namespace ContentMetadataCommon_Tests
{

    class DateTimeParser_Should : public ::testing::Test 
    {
    protected:
        std::unique_ptr<ContentMetadataCommon::IDateTimeParser> m_sut = std::make_unique<ContentMetadataCommon::DateTimeParser>();

        std::string toIso8601(std::time_t time) 
        {
            std::stringstream ss;
            ss << std::put_time(std::gmtime(&time), "%FT%TZ");
            return ss.str();
        }

        std::string toPgSql(std::time_t time)
        {
            std::stringstream ss;
            ss << std::put_time(std::gmtime(&time), "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }
    };

}