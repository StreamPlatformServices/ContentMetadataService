#pragma once
#include "../StdAfx.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
		class IDateTimeParser
		{
		public:
			virtual ~IDateTimeParser() = default;
			
			virtual void parseIso8601(const std::string& a_date_time) = 0;
			virtual void setTimePoint(std::chrono::system_clock::time_point a_time_point) noexcept = 0;

			virtual std::chrono::system_clock::time_point  getTimePoint() const noexcept = 0;
			virtual std::time_t getTimeT() const noexcept = 0;
			virtual std::string getIso8601String() const noexcept = 0;
		};
	}
}