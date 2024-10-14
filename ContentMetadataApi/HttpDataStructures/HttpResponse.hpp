#pragma once
#include "HttpStatusCode.hpp"

namespace ContentMetadataApi
{
    struct HttpResponse
    {
        HttpStatusCode                               m_status_code;
        std::unordered_map<std::string, std::string> m_headers;
        std::string                                  m_body;
    };
}