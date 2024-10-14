#pragma once
#include "HttpMethod.hpp"

namespace ContentMetadataApi
{
    struct HttpRequest
    {
        HttpMethod                                    m_method;
        std::string                                   m_root_endpoint;
        std::vector<std::string>                      m_routing_path;
        std::unordered_map<std::string, std::string>  m_headers;
        std::unordered_map<std::string, std::string>  m_query_params;
        std::string                                   m_body;
    };   

}