#pragma once
#include "DataValidationVisitor.hpp"

namespace ContentMetadataApi
{
	namespace Visitors
	{
		DataValidationVisitor::DataValidationVisitor(
			std::vector<std::string>&errors,
			std::shared_ptr<Parsers::IGuidParser> a_guid_parser) noexcept
			: m_errors(errors)
			, m_guid_parser(a_guid_parser)
		{
		}

		void DataValidationVisitor::visit(Dto::ErrorsDto& a_a_visitable) { return; }

		void DataValidationVisitor::visit(Dto::ContentDto& a_visitable) 
		{
			if (!m_guid_parser->isGuid(a_visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: ContentDto UUID is invalid.");
			}

			if (a_visitable.m_title.empty())
			{
				m_errors.push_back("Validation Error: ContentDto title cannot be empty.");
			}

			if (a_visitable.m_description.empty())
			{
				m_errors.push_back("Validation Error: ContentDto description cannot be empty.");
			}

			if (a_visitable.m_duration <= 0)
			{
				m_errors.push_back("Validation Error: ContentDto duration cannot be negative.");
			}

			if (a_visitable.m_license_rules.empty())
			{
				m_errors.push_back("Validation Error: At least one license rule is required.");
			}

			for (auto& rule : a_visitable.m_license_rules)
			{
				visit(rule);
			}

			if (a_visitable.m_content_comments.has_value())
			{
				for (auto& comment : *a_visitable.m_content_comments)
				{
					visit(comment);
				}
			}

			if (!m_guid_parser->isGuid(a_visitable.m_owner_id))
			{
				m_errors.push_back("Validation Error: ContentDto owner ID is invalid.");
			}

			if (!m_guid_parser->isGuid(a_visitable.m_video_file_id))
			{
				m_errors.push_back("Validation Error: ContentDto video file ID is invalid.");
			}

			if (!m_guid_parser->isGuid(a_visitable.m_image_file_id))
			{
				m_errors.push_back("Validation Error: ContentDto image file ID is invalid.");
			}

			if (a_visitable.m_upload_time.time_since_epoch().count() <= 0)
			{
				m_errors.push_back("Validation Error: ContentDto upload time is invalid.");
			}

			auto current_time = std::chrono::system_clock::now();
			if (a_visitable.m_upload_time > current_time)
			{
				m_errors.push_back("Validation Error: ContentDto upload time cannot be in the future.");
			}
		}

		void DataValidationVisitor::visit(Dto::ContentCommentDto& a_visitable)
		{
			if (!m_guid_parser->isGuid(a_visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: ContentCommentDto UUID is invalid.");
			}

			if (a_visitable.m_body.empty())
			{
				m_errors.push_back("Validation Error: ContentCommentDto body cannot be empty.");
			}

			if (a_visitable.m_user_name.empty())
			{
				m_errors.push_back("Validation Error: ContentCommentDto user name cannot be empty.");
			}

			if (!m_guid_parser->isGuid(a_visitable.m_content_id))
			{
				m_errors.push_back("Validation Error: ContentCommentDto content ID is invalid.");
			}

			if (a_visitable.m_creation_time.time_since_epoch().count() <= 0)
			{
				m_errors.push_back("Validation Error: ContentCommentDto creation time is invalid.");
			}

			auto current_time = std::chrono::system_clock::now();
			if (a_visitable.m_creation_time > current_time)
			{
				m_errors.push_back("Validation Error: ContentCommentDto creation time cannot be in the future.");
			}
		}

		void DataValidationVisitor::visit(Dto::LicenseRulesDto& a_visitable)
		{
			if (!m_guid_parser->isGuid(a_visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: LicenseRulesDto UUID is invalid.");
			}

			if (!m_guid_parser->isGuid(a_visitable.m_content_id))
			{
				m_errors.push_back("Validation Error: LicenseRulesDto has now information about content_id.");
			}

			if (a_visitable.m_price < 0)
			{
				m_errors.push_back("Validation Error: LicenseRulesDto price cannot be negative.");
			}

			if (a_visitable.m_type == ContentMetadataCore::Enums::LicenseType::Unknown)
			{
				m_errors.push_back("Validation Error: LicenseRulesDto type is unknown.");
			}

			if (a_visitable.m_type == ContentMetadataCore::Enums::LicenseType::Rent)
			{
				if (!a_visitable.m_duration.has_value())
				{
					m_errors.push_back("Validation Error: License duration is reuiered for license type RENT.");
				}

				if (a_visitable.m_duration.has_value())
				{
					auto duration_value = a_visitable.m_duration.value();
					if (duration_value == ContentMetadataCore::Enums::LicenseDuration::Unknown)
					{
						m_errors.push_back("Validation Error: LicenseRulesDto duration is unknown.");
					}
				}
			}
		}

		void DataValidationVisitor::visit(Dto::GuidDto& a_visitable)
		{
			if (!m_guid_parser->isGuid(a_visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: GuidDto UUID is invalid.");
			}
		}

		void DataValidationVisitor::visit(Dto::ContentsDto& a_visitable)
		{
			for (auto& content : a_visitable.m_contents)
			{
				visit(content); 
			}
		}
	}
}
