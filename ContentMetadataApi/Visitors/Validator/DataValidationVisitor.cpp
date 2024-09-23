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

		void DataValidationVisitor::visit(Dto::ErrorsDto& a_visitable) { return; }

		void DataValidationVisitor::visit(Dto::ContentDto& visitable) 
		{
			if (!m_guid_parser->isGuid(visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: ContentDto UUID is invalid.");
			}

			if (visitable.m_title.empty())
			{
				m_errors.push_back("Validation Error: ContentDto title cannot be empty.");
			}

			if (visitable.m_duration <= 0)
			{
				m_errors.push_back("Validation Error: ContentDto duration cannot be negative.");
			}

			if (visitable.m_license_rules.empty())
			{
				m_errors.push_back("Validation Error: At least one license rule is required.");
			}

			for (auto& rule : visitable.m_license_rules)
			{
				visit(rule);
			}

			if (visitable.m_content_comments.has_value())
			{
				for (auto& comment : *visitable.m_content_comments)
				{
					visit(comment);
				}
			}

			if (!m_guid_parser->isGuid(visitable.m_owner_id))
			{
				m_errors.push_back("Validation Error: ContentDto owner ID is invalid.");
			}

			if (!m_guid_parser->isGuid(visitable.m_video_file_id))
			{
				m_errors.push_back("Validation Error: ContentDto video file ID is invalid.");
			}

			if (!m_guid_parser->isGuid(visitable.m_image_file_id))
			{
				m_errors.push_back("Validation Error: ContentDto image file ID is invalid.");
			}
		}

		void DataValidationVisitor::visit(Dto::ContentCommentDto& visitable)
		{
			if (!m_guid_parser->isGuid(visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: ContentCommentDto UUID is invalid.");
			}

			if (visitable.m_body.empty())
			{
				m_errors.push_back("Validation Error: ContentCommentDto body cannot be empty.");
			}

			if (visitable.m_user_name.empty())
			{
				m_errors.push_back("Validation Error: ContentCommentDto user name cannot be empty.");
			}

			if (!m_guid_parser->isGuid(visitable.m_content_id))
			{
				m_errors.push_back("Validation Error: ContentCommentDto content ID is invalid.");
			}
		}

		void DataValidationVisitor::visit(Dto::LicenseRulesDto& visitable)
		{
			if (!m_guid_parser->isGuid(visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: LicenseRulesDto UUID is invalid.");
			}

			if (visitable.m_price < 0)
			{
				m_errors.push_back("Validation Error: LicenseRulesDto price cannot be negative.");
			}

			if (visitable.m_type == ContentMetadataCore::Enums::LicenseType::Unknown)
			{
				m_errors.push_back("Validation Error: LicenseRulesDto type is unknown.");
			}

			if (!visitable.m_duration.has_value() && visitable.m_type == ContentMetadataCore::Enums::LicenseType::Rent)
			{
				m_errors.push_back("Validation Error: License duration is reuiered for license type RENT.");
			}

			if (visitable.m_duration.has_value())
			{
				auto duration_value = visitable.m_duration.value();
				if (duration_value == ContentMetadataCore::Enums::LicenseDuration::Unknown)
				{
					m_errors.push_back("Validation Error: LicenseRulesDto duration is unknown.");
				}
			}
		}

		void DataValidationVisitor::visit(Dto::GuidDto& visitable)
		{
			if (!m_guid_parser->isGuid(visitable.m_uuid))
			{
				m_errors.push_back("Validation Error: GuidDto UUID is invalid.");
			}
		}

		void DataValidationVisitor::visit(Dto::ContentsDto& visitable)
		{
			if (visitable.m_contents.empty())
			{
				m_errors.push_back("Validation Error: ContentsDto must contain at least one ContentDto.");
			}

			for (auto& content : visitable.m_contents)
			{
				visit(content); 
			}
		}
	}
}
