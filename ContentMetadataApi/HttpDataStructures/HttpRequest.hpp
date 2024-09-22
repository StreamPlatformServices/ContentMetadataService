#pragma once

namespace ContentMetadataApi
{
    enum class HttpMethod 
    {
        Get,
        Post,
        Put,
        Delete,
        Patch,
        Head,
        Options,
        Connect,
        Trace,
        Unknown 
    };

    struct HttpRequest
    {
        HttpMethod m_method;
        std::string m_root_endpoint;
        std::vector<std::string> m_routing_path;
        std::unordered_map<std::string, std::string> m_headers;
        std::unordered_map<std::string, std::string> m_query_params;
        std::string m_body;
    };   

}