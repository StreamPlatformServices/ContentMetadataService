#pragma once
#include "../StdAfx.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
		class IGuidParser
		{
		public:
			virtual ~IGuidParser() = default;

			virtual bool isGuid(const boost::uuids::uuid& uuid) const = 0;
			virtual bool isGuid(const std::string& a_guid_string) const = 0;
			virtual boost::uuids::uuid parseGuid(const std::string& a_guid_string) const = 0;
		};
	}
}