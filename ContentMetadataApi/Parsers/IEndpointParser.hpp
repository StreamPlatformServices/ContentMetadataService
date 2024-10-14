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
			virtual auto getQueryParams() const noexcept -> std::unordered_map<std::string, std::string> = 0;
			virtual auto getRoutingPath() const noexcept -> std::vector<std::string> = 0;
			virtual auto getRootEndpoint() const noexcept -> std::string = 0;
		};
	}
}