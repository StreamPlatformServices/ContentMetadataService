#pragma once

#include "LicenseRules.hpp"
#include "ContentComment.hpp"

namespace ContentMetadataCore
{
    namespace Entities
    {
        struct Content
        {
            boost::uuids::uuid                     m_uuid = boost::uuids::nil_uuid();
            std::string                            m_title = "";
            std::chrono::system_clock::time_point  m_upload_time = std::chrono::system_clock::now();
            int                                    m_duration = 0;
            std::string                            m_description = "";
            std::vector<LicenseRules>              m_license_rules;
            std::vector<ContentComment>            m_content_comments;
            boost::uuids::uuid                     m_owner_id = boost::uuids::nil_uuid();
            boost::uuids::uuid                     m_video_file_id = boost::uuids::nil_uuid();
            boost::uuids::uuid                     m_image_file_id = boost::uuids::nil_uuid();
        };
    }
}