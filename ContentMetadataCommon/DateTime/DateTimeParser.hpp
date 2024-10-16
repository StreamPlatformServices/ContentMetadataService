#pragma once
#include "IDateTimeParser.hpp"
#include <shared_mutex>
#include <mutex>

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

		auto getTimePoint() noexcept -> std::chrono::system_clock::time_point override;
		auto getTimeT() noexcept -> std::time_t override;
		auto getIso8601String() noexcept -> std::string override;
		auto getPgSqlDatabaseFormat() noexcept -> std::string override;

	private:
		std::shared_mutex m_time_point_mutex;
		std::chrono::system_clock::time_point m_time_point;
	};
}