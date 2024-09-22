#pragma once
#include "../StdAfx.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
		
		class IEndpointParser
		{
		public:
			virtual ~IEndpointParser() = default;

			virtual void parseEndpoint(const std::string& a_endpoint) = 0;
			virtual std::unordered_map<std::string, std::string> getQueryParams() const noexcept = 0;
			virtual std::vector<std::string> getRoutingPath() const noexcept = 0;
		};
	}
}