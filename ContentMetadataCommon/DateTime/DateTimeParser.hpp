#pragma once
#include "IDateTimeParser.hpp"
#include <shared_mutex>

namespace ContentMetadataCommon
{
	class DateTimeParser : public IDateTimeParser
	{
	public:
		DateTimeParser() = default;
		DateTimeParser(const DateTimeParser&) = delete;
		DateTimeParser(DateTimeParser&&) = delete;

		DateTimeParser& operator=(const DateTimeParser&) = delete;
		DateTimeParser& operator=(DateTimeParser&&) = delete;

		void parsePgSqlFormat(const std::string& a_date_time) override;
		void parseIso8601(const std::string& a_date_time) override;
		void setTimePoint(std::chrono::system_clock::time_point a_time_point) noexcept override;

		std::chrono::system_clock::time_point  getTimePoint() noexcept override;
		std::time_t getTimeT() noexcept override;
		std::string getIso8601String() noexcept override;
		std::string getPgSqlDatabaseFormat() noexcept override;

	private:
		std::shared_mutex m_time_point_mutex;
		std::chrono::system_clock::time_point m_time_point;
	};
}