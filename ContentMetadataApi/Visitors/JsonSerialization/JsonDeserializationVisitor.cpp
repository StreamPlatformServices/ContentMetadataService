#pragma once
#include "JsonDeserializationVisitor.hpp"
#include "JsonKeys.hpp"

namespace ContentMetadataApi
{
	namespace Visitors
	{
		JsonDeserializationVisitor::JsonDeserializationVisitor(
			const std::string& json_string,
			std::shared_ptr<ContentMetadataCommon::IDateTimeParser> a_date_time_parser,
			std::shared_ptr<Parsers::IGuidParser> a_guid_parser) noexcept
			: m_json_string(json_string)
			, m_date_time_parser(a_date_time_parser)
			, m_guid_parser(a_guid_parser)
		{
		}

		void JsonDeserializationVisitor::visit(Dto::ErrorsDto& a_visitable)
		{
			throw std::runtime_error("JsonDeserializationVisitor::visit is not implemented for ErrorsDto");
		}

		void JsonDeserializationVisitor::visit(Dto::ContentsDto& a_visitable)
		{
			throw std::runtime_error("JsonDeserializationVisitor::visit is not implemented for ContentsDto");
		}

		void JsonDeserializationVisitor::visit(Dto::GuidDto& a_visitable)
		{
			throw std::runtime_error("JsonDeserializationVisitor::visit is not implemented for GuidDto");
		}

		void JsonDeserializationVisitor::visit(Dto::ContentDto& visitable)
		{
			rapidjson::Document document;

			if (document.Parse(m_json_string.c_str()).HasParseError())
			{
				std::stringstream ss;
				ss << "JSON parse error: " << rapidjson::GetParseError_En(document.GetParseError())
					<< " (offset " << document.GetErrorOffset() << ")" << std::endl;
				std::cerr << ss.str();
				throw std::invalid_argument(ss.str());
			}

			visitable = deserializeContent(document);
		}

		void JsonDeserializationVisitor::visit(Dto::ContentCommentDto& visitable)
		{
			rapidjson::Document document;

			if (document.Parse(m_json_string.c_str()).HasParseError())
			{
				std::stringstream ss;
				ss << "JSON parse error: " << rapidjson::GetParseError_En(document.GetParseError())
					<< " (offset " << document.GetErrorOffset() << ")" << std::endl;
				std::cerr << ss.str();
				throw std::invalid_argument(ss.str());
			}

			visitable = deserializeContentComment(document);
		}

		void JsonDeserializationVisitor::visit(Dto::LicenseRulesDto& visitable)
		{
			rapidjson::Document document;

			if (document.Parse(m_json_string.c_str()).HasParseError())
			{
				std::stringstream ss;
				ss << "JSON parse error: " << rapidjson::GetParseError_En(document.GetParseError())
					<< " (offset " << document.GetErrorOffset() << ")" << std::endl;
				std::cerr << ss.str();
				throw std::invalid_argument(ss.str());
			}

			visitable = deserializeLicenseRules(document);
		}


		auto JsonDeserializationVisitor::deserializeContent(const rapidjson::Value& content_json_value) const -> Dto::ContentDto
		{
			Dto::ContentDto result;

			if (content_json_value.HasMember(JsonKeys::UUID) && content_json_value[JsonKeys::UUID].IsString())
			{
				try
				{
					result.m_uuid = m_guid_parser->parseGuid(content_json_value[JsonKeys::UUID].GetString());
				}
				catch (const std::exception& e)
				{
					std::stringstream ss;
					ss << "Invalid uuid exception: " << e.what();
					throw std::invalid_argument(e.what());
				}
			}
			else
			{
				throw std::invalid_argument("Content uuid is requierd!");
			}

			if (content_json_value.HasMember(JsonKeys::TITLE) && content_json_value[JsonKeys::TITLE].IsString())
			{
				result.m_title = content_json_value[JsonKeys::TITLE].GetString();
			}
			else
			{
				throw std::invalid_argument("Title is requierd!");
			}

			if (content_json_value.HasMember(JsonKeys::UPLOAD_TIME) && content_json_value[JsonKeys::UPLOAD_TIME].IsString())
			{
				auto&& upload_time_str = content_json_value[JsonKeys::UPLOAD_TIME].GetString();
				m_date_time_parser->parseIso8601(upload_time_str);
				result.m_upload_time = m_date_time_parser->getTimePoint();
			}
			else
			{
				throw std::invalid_argument("Upload time is requierd!");
			}

			if (content_json_value.HasMember(JsonKeys::DURATION) && content_json_value[JsonKeys::DURATION].IsInt())
			{
				result.m_duration = content_json_value[JsonKeys::DURATION].GetInt();
			}
			else
			{
				throw std::invalid_argument("Duration is requierd!");
			}

			if (content_json_value.HasMember(JsonKeys::DESCRIPTION) && content_json_value[JsonKeys::DESCRIPTION].IsString())
			{
				result.m_description = content_json_value[JsonKeys::DESCRIPTION].GetString();
			}
			else
			{
				throw std::invalid_argument("Description is requierd!");
			}

			if (content_json_value.HasMember(JsonKeys::LICENSE_RULES) && content_json_value[JsonKeys::LICENSE_RULES].IsArray())
			{
				for (auto& rules : content_json_value[JsonKeys::LICENSE_RULES].GetArray())
				{
					auto license_rules = deserializeLicenseRules(rules);
					result.m_license_rules.push_back(license_rules);
				}
			}
			else
			{
				throw std::invalid_argument("License rules are required!");
			}

			if (content_json_value.HasMember(JsonKeys::CONTENT_COMMENTS) && content_json_value[JsonKeys::CONTENT_COMMENTS].IsArray())
			{
				result.m_content_comments = std::vector<ContentMetadataApi::Dto::ContentCommentDto>();
				for (auto& comment : content_json_value[JsonKeys::CONTENT_COMMENTS].GetArray())
				{
					auto content_comment = deserializeContentComment(comment);
					result.m_content_comments->push_back(content_comment);
				}
			}

			if (content_json_value.HasMember(JsonKeys::OWNER_ID) && content_json_value[JsonKeys::OWNER_ID].IsString())
			{
				auto&& owner_id_str = content_json_value[JsonKeys::OWNER_ID].GetString();
				try
				{
					result.m_owner_id = m_guid_parser->parseGuid(owner_id_str);
				}
				catch (const std::exception& e)
				{
					std::stringstream ss;
					ss << "Invalid owner file id exception: " << e.what();
					throw std::invalid_argument(ss.str());
				}
			}
			else
			{
				throw std::invalid_argument("Owner id is requierd!");
			}

			if (content_json_value.HasMember(JsonKeys::VIDEO_FILE_ID) && content_json_value[JsonKeys::VIDEO_FILE_ID].IsString())
			{
				
				auto&& video_file_id_str = content_json_value[JsonKeys::VIDEO_FILE_ID].GetString();
				try
				{
					result.m_video_file_id = m_guid_parser->parseGuid(video_file_id_str);
				}
				catch (const std::exception& e)
				{
					std::stringstream ss;
					ss << "Invalid video file id exception: " << e.what();
					throw std::invalid_argument(ss.str());
				}
			}
			else
			{
				throw std::invalid_argument("Video file id is requierd!");
			}

			if (content_json_value.HasMember(JsonKeys::IMAGE_FILE_ID) && content_json_value[JsonKeys::IMAGE_FILE_ID].IsString())
			{
				auto&& image_file_id_str = content_json_value[JsonKeys::IMAGE_FILE_ID].GetString();
				try
				{
					result.m_image_file_id = m_guid_parser->parseGuid(image_file_id_str);
				}
				catch (const std::exception& e)
				{
					std::stringstream ss;
					ss << "Invalid image file id exception: " << e.what();
					throw std::invalid_argument(ss.str());
				}
			}
			else
			{
				throw std::invalid_argument("Image file id is requierd!");
			}

			return result;
		}

		auto JsonDeserializationVisitor::deserializeContentComment(const rapidjson::Value& content_comment_json_value) const -> Dto::ContentCommentDto
		{
			Dto::ContentCommentDto result;

			if (content_comment_json_value.HasMember(JsonKeys::UUID) && content_comment_json_value[JsonKeys::UUID].IsString())
			{
				auto&& uuid_str = content_comment_json_value[JsonKeys::UUID].GetString();
				try
				{
					result.m_uuid = m_guid_parser->parseGuid(uuid_str);
				}
				catch (const std::exception& e)
				{
					std::stringstream ss;
					ss << "Invalid comment uuid exception: " << e.what();
					throw std::invalid_argument(ss.str());
				}
			}
			else
			{
				throw std::invalid_argument("Comment uuid is requierd!");
			}

			if (content_comment_json_value.HasMember(JsonKeys::BODY) && content_comment_json_value[JsonKeys::BODY].IsString())
			{
				result.m_body = content_comment_json_value[JsonKeys::BODY].GetString();
			}
			else
			{
				throw std::invalid_argument("Comment body is requierd!");
			}

			if (content_comment_json_value.HasMember(JsonKeys::CREATION_TIME) && content_comment_json_value[JsonKeys::CREATION_TIME].IsString())
			{
				auto&& creation_time_str = content_comment_json_value[JsonKeys::CREATION_TIME].GetString();
				m_date_time_parser->parseIso8601(creation_time_str);
				result.m_creation_time = m_date_time_parser->getTimePoint();
			}
			else
			{
				throw std::invalid_argument("Comment creation time is requierd!");
			}

			if (content_comment_json_value.HasMember(JsonKeys::CONTENT_ID) && content_comment_json_value[JsonKeys::CONTENT_ID].IsString())
			{
				auto&& content_id_str = content_comment_json_value[JsonKeys::CONTENT_ID].GetString();
				try
				{
					result.m_content_id = m_guid_parser->parseGuid(content_id_str);
				}
				catch (const std::exception& e)
				{
					std::stringstream ss;
					ss << "Invalid content uuid exception: " << e.what();
					throw std::invalid_argument(ss.str());
				}
			}
			else
			{
				throw std::invalid_argument("Content id is requierd!");
			}

			if (content_comment_json_value.HasMember(JsonKeys::USER_NAME) && content_comment_json_value[JsonKeys::USER_NAME].IsString())
			{
				result.m_user_name = content_comment_json_value[JsonKeys::USER_NAME].GetString();
			}
			else
			{
				throw std::invalid_argument("Comment author name is requierd!");
			}

			return result;
		}

		auto JsonDeserializationVisitor::deserializeLicenseRules(const rapidjson::Value& license_rules_json_value) const -> Dto::LicenseRulesDto
		{
			Dto::LicenseRulesDto result;

			if (license_rules_json_value.HasMember(JsonKeys::UUID) && license_rules_json_value[JsonKeys::UUID].IsString())
			{
				auto&& uuid_str = license_rules_json_value[JsonKeys::UUID].GetString();
				try
				{
					result.m_uuid = m_guid_parser->parseGuid(uuid_str);
				}
				catch (const std::exception& e)
				{
					std::stringstream ss;
					ss << "Invalid license rules uuid exception: " << e.what();
					throw std::invalid_argument(ss.str());
				}
			}
			else
			{
				throw std::invalid_argument("License uuid is requierd!");
			}

			if (license_rules_json_value.HasMember(JsonKeys::PRICE) && license_rules_json_value[JsonKeys::PRICE].IsInt())
			{
				result.m_price = license_rules_json_value[JsonKeys::PRICE].GetInt();
			}
			else
			{
				throw std::invalid_argument("License price is requierd!");
			}

			if (license_rules_json_value.HasMember(JsonKeys::TYPE) && license_rules_json_value[JsonKeys::TYPE].IsUint())
			{
				auto type_int = license_rules_json_value[JsonKeys::TYPE].GetUint();
				
				if (!isLicenseTypeEnumInRange(type_int))
				{
					throw std::invalid_argument("Enum type parameter is not in range.");
				}

				result.m_type = static_cast<ContentMetadataCore::Enums::LicenseType>(type_int);
			}
			else
			{
				throw std::invalid_argument("License type is missing or invalid!");
			}

			if (license_rules_json_value.HasMember(JsonKeys::DURATION) && license_rules_json_value[JsonKeys::DURATION].IsUint())
			{
				auto duration_int = license_rules_json_value[JsonKeys::DURATION].GetUint();

				if (!isLicenseDurationEnumInRange(duration_int))
				{
					throw std::invalid_argument("Enum duration parameter is not in range.");
				}

				result.m_duration = static_cast<ContentMetadataCore::Enums::LicenseDuration>(duration_int);
			}
			else
			{
				throw std::invalid_argument("License type is missing or invalid!");
			}

			return result;
		}

		bool JsonDeserializationVisitor::isLicenseTypeEnumInRange(const int a_enum_int_value) const noexcept
		{
			if (a_enum_int_value < static_cast<int>(ContentMetadataCore::Enums::LicenseType::Unknown) ||
				a_enum_int_value > static_cast<int>(ContentMetadataCore::Enums::LicenseType::Rent))
			{
				return false;
			}

			return true;
		}

		bool JsonDeserializationVisitor::isLicenseDurationEnumInRange(const int a_enum_int_value) const noexcept
		{
			if (a_enum_int_value < static_cast<int>(ContentMetadataCore::Enums::LicenseType::Unknown) ||
				a_enum_int_value > static_cast<int>(ContentMetadataCore::Enums::LicenseType::Rent))
			{
				return false;
			}

			return true;
		}
	}
}
