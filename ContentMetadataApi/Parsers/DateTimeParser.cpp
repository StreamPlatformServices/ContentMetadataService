#pragma once
#include "DateTimeParser.hpp"

namespace
{
	constexpr auto ISO_8601_DATETIME_FORMAT = "%Y-%m-%dT%H:%M:%SZ";
}

namespace ContentMetadataApi
{
	namespace Parsers
	{

		void DateTimeParser::parseIso8601(const std::string& a_date_time)
		{
			std::tm tm = {};
			std::istringstream ss(a_date_time);

			ss >> std::get_time(&tm, ISO_8601_DATETIME_FORMAT);

			if (ss.fail()) {
				throw std::runtime_error("Wrong datetime format. Should met ISO 8601");
			}

			m_time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
		}

		void DateTimeParser::setTimePoint(std::chrono::system_clock::time_point a_time_point) noexcept
		{
			m_time_point = a_time_point;
		}

		std::chrono::system_clock::time_point  DateTimeParser::getTimePoint() const noexcept
		{ 
			return m_time_point; 
		}

		std::time_t  DateTimeParser::getTimeT() const noexcept
		{
			return std::chrono::system_clock::to_time_t(m_time_point);
		}

		std::string DateTimeParser::getIso8601String() const noexcept
		{
			auto time_t_value = getTimeT();

			std::ostringstream oss;
			oss << std::put_time(std::gmtime(&time_t_value), ISO_8601_DATETIME_FORMAT);

			return oss.str();
		}
	}
}