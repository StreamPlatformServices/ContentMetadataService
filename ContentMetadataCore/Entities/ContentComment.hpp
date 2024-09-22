#pragma once

#include "../StdAfx.hpp"

namespace ContentMetadataCore
{
    namespace Entities
    {
        struct CONTENT_METADATA_CORE_EXPORT ContentComment
        {
            boost::uuids::uuid m_uuid = boost::uuids::nil_uuid();
            std::string m_body;
            std::chrono::system_clock::time_point m_creation_time = std::chrono::system_clock::now();
            boost::uuids::uuid m_content_id = boost::uuids::nil_uuid();
            std::string m_user_name;
        };
    }
}