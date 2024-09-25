#pragma once
#include "JsonKeys.hpp"
#include "JsonSerializationVisitor.hpp"

namespace ContentMetadataApi
{
    namespace Visitors
    {
        JsonSerializationVisitor::JsonSerializationVisitor(
            std::string& a_json_string,
            std::shared_ptr<Parsers::IDateTimeParser> a_date_time_parser) noexcept
            : m_json_string(a_json_string)
            , m_date_time_parser(a_date_time_parser)
        {
        }

        void JsonSerializationVisitor::visit(Dto::ErrorsDto& a_visitable)
        {
            rapidjson::Document doc;
            doc.SetObject();
            auto& allocator = doc.GetAllocator();

            rapidjson::Value errors_array(rapidjson::kArrayType);
            for (const auto& error : a_visitable.m_errors)
            {
                errors_array.PushBack(rapidjson::Value().SetString(error.c_str(), allocator), allocator);
            }

            doc.AddMember(rapidjson::Value(JsonKeys::ERRORS, allocator), errors_array, allocator);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            doc.Accept(writer);

            m_json_string = buffer.GetString();
        }


        void JsonSerializationVisitor::visit(Dto::ContentDto& a_visitable)
        {
            rapidjson::Document doc;
            doc.SetObject();
            auto& allocator = doc.GetAllocator();

            auto json_value = serializeFrom(a_visitable, allocator);
            doc.Swap(json_value);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            if (!doc.Accept(writer))
            {
                throw std::runtime_error("Failed to write content to json buffer.");
            }

            m_json_string = buffer.GetString();
        }

        void JsonSerializationVisitor::visit(Dto::ContentCommentDto& a_visitable)
        {
            rapidjson::Document doc;
            doc.SetObject();
            auto& allocator = doc.GetAllocator();

            auto json_value = serializeFrom(a_visitable, allocator);
            doc.Swap(json_value);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            if (!doc.Accept(writer))
            {
                throw std::runtime_error("Failed to write content to json buffer.");
            }

            m_json_string = buffer.GetString();
        }

        void JsonSerializationVisitor::visit(Dto::LicenseRulesDto& a_visitable)
        {
            rapidjson::Document doc;
            doc.SetObject();
            auto& allocator = doc.GetAllocator();

            auto json_value = serializeFrom(a_visitable, allocator);
            doc.Swap(json_value);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            if (!doc.Accept(writer))
            {
                throw std::runtime_error("Failed to write content to json buffer.");
            }

            m_json_string = buffer.GetString();
        }

        void JsonSerializationVisitor::visit(Dto::GuidDto& a_visitable)
        {
            rapidjson::Document doc;
            doc.SetObject();
            auto& allocator = doc.GetAllocator();

            std::string uuid_str = boost::uuids::to_string(a_visitable.m_uuid);

            doc.AddMember(rapidjson::Value(JsonKeys::UUID, allocator), rapidjson::Value().SetString(uuid_str.c_str(), allocator), allocator);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            if (!doc.Accept(writer))
            {
                throw std::runtime_error("Failed to write content to json buffer.");
            }

            m_json_string = buffer.GetString();
        }

        void JsonSerializationVisitor::visit(Dto::ContentsDto& a_visitable)
        {
            rapidjson::Document doc;
            doc.SetObject();
            auto& allocator = doc.GetAllocator();

            rapidjson::Value contents_array(rapidjson::kArrayType);
            for (auto& content : a_visitable.m_contents)
            {
                auto content_value = serializeFrom(content, allocator);
                contents_array.PushBack(content_value, allocator);
            }

            doc.AddMember(rapidjson::Value(JsonKeys::CONTENTS, allocator), contents_array, allocator);

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            if (!doc.Accept(writer))
            {
                throw std::runtime_error("Failed to write content to json buffer.");
            }

            m_json_string = buffer.GetString();
        }

        rapidjson::Value JsonSerializationVisitor::serializeFrom(Dto::ContentDto& a_content_dto, rapidjson::Document::AllocatorType& allocator) const
        {
            rapidjson::Value doc(rapidjson::kObjectType);

            doc.AddMember(rapidjson::Value(JsonKeys::UUID, allocator), rapidjson::Value().SetString(boost::uuids::to_string(a_content_dto.m_uuid).c_str(), allocator), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::TITLE, allocator), rapidjson::Value().SetString(a_content_dto.m_title.c_str(), allocator), allocator);

            m_date_time_parser->setTimePoint(a_content_dto.m_upload_time);
            auto&& upload_time = m_date_time_parser->getIso8601String();
            doc.AddMember(rapidjson::Value(JsonKeys::UPLOAD_TIME, allocator), rapidjson::Value().SetString(upload_time.c_str(), allocator), allocator);

            doc.AddMember(rapidjson::Value(JsonKeys::DURATION, allocator), rapidjson::Value(a_content_dto.m_duration).Move(), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::DESCRIPTION, allocator), rapidjson::Value().SetString(a_content_dto.m_description.c_str(), allocator), allocator);

            rapidjson::Value license_rules_array(rapidjson::kArrayType);
            for (auto& rule : a_content_dto.m_license_rules)
            {
                auto rule_value = serializeFrom(rule, allocator);
                license_rules_array.PushBack(rule_value, allocator);
            }
            doc.AddMember(rapidjson::Value(JsonKeys::LICENSE_RULES, allocator), license_rules_array, allocator);
            
            if (a_content_dto.m_content_comments)
            {
                rapidjson::Value comments_array(rapidjson::kArrayType);
                for (auto& comment : *a_content_dto.m_content_comments)
                {
                    auto comment_value = serializeFrom(comment, allocator);
                    comments_array.PushBack(comment_value, allocator);
                }
                doc.AddMember(rapidjson::Value(JsonKeys::CONTENT_COMMENTS, allocator), comments_array, allocator);
            }

            doc.AddMember(rapidjson::Value(JsonKeys::OWNER_ID, allocator), rapidjson::Value().SetString(boost::uuids::to_string(a_content_dto.m_owner_id).c_str(), allocator), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::VIDEO_FILE_ID, allocator), rapidjson::Value().SetString(boost::uuids::to_string(a_content_dto.m_video_file_id).c_str(), allocator), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::IMAGE_FILE_ID, allocator), rapidjson::Value().SetString(boost::uuids::to_string(a_content_dto.m_image_file_id).c_str(), allocator), allocator);

            return doc;
        }

        rapidjson::Value JsonSerializationVisitor::serializeFrom(Dto::ContentCommentDto& a_content_comment_dto, rapidjson::Document::AllocatorType& allocator) const
        {
            rapidjson::Value doc(rapidjson::kObjectType);

            doc.AddMember(rapidjson::Value(JsonKeys::UUID, allocator), rapidjson::Value().SetString(boost::uuids::to_string(a_content_comment_dto.m_uuid).c_str(), allocator), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::BODY, allocator), rapidjson::Value().SetString(a_content_comment_dto.m_body.c_str(), allocator), allocator);

            m_date_time_parser->setTimePoint(a_content_comment_dto.m_creation_time);
            auto&& creation_time = m_date_time_parser->getIso8601String();
            doc.AddMember(rapidjson::Value(JsonKeys::CREATION_TIME, allocator), rapidjson::Value().SetString(creation_time.c_str(), allocator), allocator);

            doc.AddMember(rapidjson::Value(JsonKeys::CONTENT_ID, allocator), rapidjson::Value().SetString(boost::uuids::to_string(a_content_comment_dto.m_content_id).c_str(), allocator), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::USER_NAME, allocator), rapidjson::Value().SetString(a_content_comment_dto.m_user_name.c_str(), allocator), allocator);

            return doc;
        }

        rapidjson::Value JsonSerializationVisitor::serializeFrom(Dto::LicenseRulesDto& a_license_rules_dto, rapidjson::Document::AllocatorType& allocator) const
        {
            rapidjson::Value doc(rapidjson::kObjectType);

            doc.AddMember(rapidjson::Value(JsonKeys::UUID, allocator), rapidjson::Value().SetString(boost::uuids::to_string(a_license_rules_dto.m_uuid).c_str(), allocator), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::PRICE, allocator), rapidjson::Value(a_license_rules_dto.m_price).Move(), allocator);
            doc.AddMember(rapidjson::Value(JsonKeys::TYPE, allocator), rapidjson::Value(static_cast<int>(a_license_rules_dto.m_type)).Move(), allocator);

            if (a_license_rules_dto.m_duration.has_value())
            {
                doc.AddMember(rapidjson::Value(JsonKeys::DURATION, allocator), rapidjson::Value(static_cast<int>(a_license_rules_dto.m_duration.value())).Move(), allocator);
            }

            return doc;
        }

    }
}
