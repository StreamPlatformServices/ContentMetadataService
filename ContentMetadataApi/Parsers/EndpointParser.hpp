#pragma once
#include "IEndpointParser.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
		class CONTENT_METADATA_API_EXPORT EndpointParser : public IEndpointParser
		{
		public:
			EndpointParser() = default;
			EndpointParser(const EndpointParser&) = delete;
			EndpointParser(EndpointParser&&) = delete;

			EndpointParser& operator=(const EndpointParser&) = delete;
			EndpointParser& operator=(EndpointParser&&) = delete;

            void parseEndpoint(const std::string& a_endpoint) override;
            auto getQueryParams() const noexcept -> std::unordered_map<std::string, std::string>  override;
            auto getRoutingPath() const noexcept -> std::vector<std::string> override;
			auto getRootEndpoint() const noexcept -> std::string override;

        private:
            void parseRoutingPath(const std::string& a_path);
            void parseQueryParams(const std::string& a_query);

		private:
			std::unordered_map<std::string, std::string> m_query_params;
			std::vector<std::string> m_routing_path;
			std::string m_root_endpoint;

		};
	}
}