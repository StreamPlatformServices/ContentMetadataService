#pragma once
#include "IGuidParser.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
		class CONTENT_METADATA_API_EXPORT GuidParser : public IGuidParser
		{
		public:
			GuidParser() = default;
			GuidParser(const GuidParser&) = delete;
			GuidParser(GuidParser&&) = delete;

			GuidParser& operator=(const GuidParser&) = delete;
			GuidParser& operator=(GuidParser&&) = delete;

			bool isGuid(const boost::uuids::uuid& uuid) const override;
			bool isGuid(const std::string& a_guid_string) const override;
			auto parseGuid(const std::string& a_guid_string) const -> boost::uuids::uuid  override;
		};
	}
}