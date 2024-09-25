#pragma once
#include "DateTimeParser_Should.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(DateTimeParser_Should, SetAndGetTimePointCorrectly)
    {
        auto now = std::chrono::system_clock::now();

        m_sut->setTimePoint(now);

        EXPECT_EQ(m_sut->getTimePoint(), now);
    }

    TEST_F(DateTimeParser_Should, GetTimeTCorrectly)
    {
        auto now = std::chrono::system_clock::now();
        m_sut->setTimePoint(now);

        auto time_t_value = std::chrono::system_clock::to_time_t(now);

        EXPECT_EQ(m_sut->getTimeT(), time_t_value);
    }

    TEST_F(DateTimeParser_Should, GetIso8601StringCorrectly)
    {
        auto now = std::chrono::system_clock::now();
        m_sut->setTimePoint(now);

        auto time_t_value = m_sut->getTimeT();
        auto expected_iso8601 = toIso8601(time_t_value);

        EXPECT_EQ(m_sut->getIso8601String(), expected_iso8601);
    }


    TEST_F(DateTimeParser_Should, ParseIso8601Correctly)
    {
        std::string iso8601_date = "2023-09-24T15:30:00Z";

        m_sut->parseIso8601(iso8601_date);

        auto parsed_time_point = m_sut->getTimePoint();
        auto time_t_value = std::chrono::system_clock::to_time_t(parsed_time_point);

        struct std::tm tm = {};
        std::istringstream ss(iso8601_date);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

        std::time_t expected_time = std::mktime(&tm);
        EXPECT_EQ(time_t_value, expected_time);
    }

    TEST_F(DateTimeParser_Should, HandleInvalidIso8601String)
    {
        std::string invalid_iso8601_date = "invalid-date";

        EXPECT_THROW(m_sut->parseIso8601(invalid_iso8601_date), std::invalid_argument);
    }
}