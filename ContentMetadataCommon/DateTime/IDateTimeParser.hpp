#pragma once
#include <chrono>

namespace ContentMetadataCommon
{
	class IDateTimeParser
	{
	public:
		virtual ~IDateTimeParser() = default;
			
		virtual void parsePgSqlFormat(const std::string& a_date_time) = 0;
		virtual void parseIso8601(const std::string& a_date_time) = 0;
		virtual void setTimePoint(std::chrono::system_clock::time_point a_time_point) noexcept = 0;

		virtual auto getTimePoint() noexcept -> std::chrono::system_clock::time_point = 0;
		virtual auto getTimeT() noexcept -> std::time_t = 0;
		virtual auto getIso8601String() noexcept -> std::string = 0;
		virtual auto getPgSqlDatabaseFormat() noexcept -> std::string = 0;
	};
}