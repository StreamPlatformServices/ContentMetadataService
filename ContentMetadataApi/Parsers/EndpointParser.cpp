#pragma once
#include "EndpointParser.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
        void EndpointParser::parseEndpoint(const std::string& a_endpoint)
        {
            size_t query_pos = a_endpoint.find('?');

            std::string path = a_endpoint.substr(0, query_pos);

            std::unordered_map<std::string, std::string> query_params;

            if (query_pos != std::string::npos) 
            {
                std::string query = a_endpoint.substr(query_pos + 1);
                parseQueryParams(query);
            }

            parseRoutingPath(path);
        }

        void EndpointParser::parseRoutingPath(const std::string& a_path)
        {
            boost::split(m_routing_path, a_path, boost::is_any_of("/"), boost::token_compress_on);
        }

        //TODO: UT CASE: std::string query = "param1=value1&param2=&flag&param3=value3&";
        //TODO: UT CASE: std::string query = "param1=value1&param2=&flag&param3=value3&&s&s&&&=&"; -> this needs to be validated
        void EndpointParser::parseQueryParams(const std::string& a_query)
        {
            std::istringstream query_stream(a_query);
            std::string param;

            while (std::getline(query_stream, param, '&'))
            {
                size_t pos = param.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = param.substr(0, pos);
                    std::string value = param.substr(pos + 1);
                    m_query_params[key] = value;
                }
                else if(!param.empty())
                {
                    m_query_params[param] = "";
                }
            }
        }

        std::unordered_map<std::string, std::string> EndpointParser::getQueryParams() const noexcept
        {
            return m_query_params;
        }

        std::vector<std::string> EndpointParser::getRoutingPath() const noexcept
        {
            return m_routing_path;
        }
	
	}
}