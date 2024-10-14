#pragma once
#include "DateTimeParser.hpp"

namespace
{
	constexpr auto ISO_8601_DATETIME_FORMAT = "%Y-%m-%dT%H:%M:%SZ";
	constexpr auto PG_SQL_DATETIME_FORMAT = "%Y-%m-%d %H:%M:%S";
}

namespace ContentMetadataCommon
{
	void DateTimeParser::parseIso8601(const std::string& a_date_time)
	{
		std::tm tm = {};
		std::istringstream ss(a_date_time);

		ss >> std::get_time(&tm, ISO_8601_DATETIME_FORMAT);

		if (ss.fail()) 
		{
			throw std::invalid_argument("Wrong datetime format. Should met ISO 8601");
		}

		std::unique_lock lock(m_time_point_mutex);
		m_time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	}

	void DateTimeParser::parsePgSqlFormat(const std::string& a_date_time)
	{
		std::tm tm = {};
		std::istringstream ss(a_date_time);

		ss >> std::get_time(&tm, PG_SQL_DATETIME_FORMAT);

		if (ss.fail()) 
		{
			throw std::invalid_argument("Wrong datetime format. Should met ISO 8601");
		}

		std::unique_lock lock(m_time_point_mutex);
		m_time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	}

	void DateTimeParser::setTimePoint(std::chrono::system_clock::time_point a_time_point) noexcept
	{
		std::unique_lock lock(m_time_point_mutex);
		m_time_point = a_time_point;
	}

	auto DateTimeParser::getTimePoint() noexcept -> std::chrono::system_clock::time_point
	{ 
		std::shared_lock lock(m_time_point_mutex);
		return m_time_point; 
	}

	auto DateTimeParser::getTimeT() noexcept -> std::time_t
	{
		std::shared_lock lock(m_time_point_mutex);
		return std::chrono::system_clock::to_time_t(m_time_point);
	}

	auto DateTimeParser::getIso8601String() noexcept -> std::string
	{
		auto time_t_value = getTimeT();

		std::ostringstream oss;
		oss << std::put_time(std::gmtime(&time_t_value), ISO_8601_DATETIME_FORMAT);

		return oss.str();
	}

	auto DateTimeParser::getPgSqlDatabaseFormat() noexcept -> std::string
	{
		auto time_t_value = getTimeT();
		std::tm tm = *std::gmtime(&time_t_value);

		std::ostringstream oss;
		oss << std::put_time(&tm, PG_SQL_DATETIME_FORMAT);
		return oss.str();
	}
	
}