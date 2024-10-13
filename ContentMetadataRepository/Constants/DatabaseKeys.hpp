#pragma once

namespace ContentMetadataRepository
{
    namespace DatabaseKeys
    {
        constexpr auto CONTENT_TABLE_NAME = "content";
        constexpr auto LICENSE_RULES_TABLE_NAME = "license_rules";
        constexpr auto CONTENT_COMMENT_TABLE_NAME = "content_comment";

        //content
        constexpr auto UUID_COLUMN_NAME = "uuid";
        constexpr auto TITLE_COLUMN_NAME = "title";
        constexpr auto UPLOAD_TIME_COLUMN_NAME = "upload_time";
        constexpr auto DURATION_COLUMN_NAME = "duration";
        constexpr auto DESCRIPTION_COLUMN_NAME = "description";
        constexpr auto OWNER_ID_COLUMN_NAME = "owner_id";
        constexpr auto VIDEO_FILE_ID_COLUMN_NAME = "video_file_id";
        constexpr auto IMAGE_FILE_ID_COLUMN_NAME = "image_file_id";

        //license_rules
        constexpr auto PRICE_COLUMN_NAME = "price";
        constexpr auto LICENSE_TYPE_COLUMN_NAME = "license_type";
        constexpr auto LICENSE_DURATION_COLUMN_NAME = "license_duration";
        constexpr auto CONTENT_UUID_COLUMN_NAME = "content_uuid";

        //content_comment
        constexpr auto BODY_COLUMN_NAME = "body";
        constexpr auto CREATION_TIME_COLUMN_NAME = "creation_time";
        constexpr auto USER_NAME_COLUMN_NAME = "user_name";
    }
}