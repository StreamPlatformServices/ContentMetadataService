#pragma once

#include "../StdAfx.h"

namespace ContentMetadataCore
{
    namespace Entities
    {
        struct ContentComment
        {
            boost::uuids::uuid Uuid = boost::uuids::nil_uuid();
            std::string Body;
            std::chrono::system_clock::time_point CreationTime = std::chrono::system_clock::now();
            boost::uuids::uuid ContentId = boost::uuids::nil_uuid();
            std::string UserName;
        };
    }
}