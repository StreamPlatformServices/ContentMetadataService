#pragma once

#include "LicenseRules.h"
#include "ContentComment.h"

namespace ContentMetadataCore
{
    namespace Entities
    {
        struct CONTENT_METADATA_CORE_EXPORT Content
        {
            boost::uuids::uuid Uuid = boost::uuids::nil_uuid();
            std::string Title = "";
            std::chrono::system_clock::time_point UploadTime = std::chrono::system_clock::now();
            int Duration = 0;
            std::string ImageUrl = "";
            std::string Description = "";
            std::vector<LicenseRules> LicenseRules;
            std::vector<ContentComment> ContentComments;
            boost::uuids::uuid OwnerId = boost::uuids::nil_uuid();
            boost::uuids::uuid VideoFileId = boost::uuids::nil_uuid();
            boost::uuids::uuid ImageFileId = boost::uuids::nil_uuid();
        };
    }
}