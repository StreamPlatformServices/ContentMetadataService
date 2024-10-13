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

		virtual std::chrono::system_clock::time_point  getTimePoint() noexcept = 0;
		virtual std::time_t getTimeT() noexcept = 0;
		virtual std::string getIso8601String() noexcept = 0;
		virtual std::string getPgSqlDatabaseFormat() noexcept = 0;
	};
}