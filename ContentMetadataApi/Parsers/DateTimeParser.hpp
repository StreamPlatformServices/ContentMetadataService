#pragma once
#include "IDateTimeParser.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
		class CONTENT_METADATA_API_EXPORT DateTimeParser : public IDateTimeParser
		{
		public:
			DateTimeParser() = default;
			DateTimeParser(const DateTimeParser&) = delete;
			DateTimeParser(DateTimeParser&&) = delete;

			DateTimeParser& operator=(const DateTimeParser&) = delete;
			DateTimeParser& operator=(DateTimeParser&&) = delete;

			void parseIso8601(const std::string& a_date_time) override;
			void setTimePoint(std::chrono::system_clock::time_point a_time_point) noexcept override;

			std::chrono::system_clock::time_point  getTimePoint() const noexcept override;
			std::time_t getTimeT() const noexcept override;
			std::string getIso8601String() const noexcept override;

		private:
			std::chrono::system_clock::time_point m_time_point;
		};
	}
}