#include "AddContentCommentCommand.hpp"
#include "../../../DataTransferObjects/GuidDto.hpp"
#include "../../../DataTransferObjects/ErrorsDto.hpp"
#include "../../../HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		AddContentCommentCommand::AddContentCommentCommand(
			const std::string& a_request_body,
			std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract,
			std::shared_ptr<Mappers::IContentCommentDataMapper> a_content_comment_mapper,
			std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept
			: m_request_body(a_request_body)
			, m_content_comments_contract(a_content_comments_contract)
			, m_content_comment_mapper(a_content_comment_mapper)
			, m_visitor_factory(a_visitor_factory)
		{
		}

		auto AddContentCommentCommand::executeAsync() const -> boost::asio::awaitable<HttpResponse>
		{
			std::cout << "Start executing AddContentCommentCommand" << std::endl;

			HttpResponse response;
			std::vector<std::string> validation_errors;

			auto json_deserialization_visitor = m_visitor_factory->createJsonDeserializationVisitor(m_request_body);
			auto validation_visitor = m_visitor_factory->createDataValidationVisitor(validation_errors);
			

			Dto::ContentCommentDto content_comment_dto;
			content_comment_dto.accept(*json_deserialization_visitor);
			content_comment_dto.accept(*validation_visitor);

			if (!validation_errors.empty())
			{
				response.m_status_code = HttpStatusCode::BadRequest;
				response.m_headers[HeaderKeys::CONTENT_TYPE] = "application/json";
				
				auto json_serialization_visitor = m_visitor_factory->createJsonSerializationVisitor(response.m_body);
				Dto::ErrorsDto errors_dto;
				errors_dto.m_errors = std::move(validation_errors);
				errors_dto.accept(*json_serialization_visitor);
				
				co_return response;
			}

			auto content_comment_entity = m_content_comment_mapper->contentCommentEntityFrom(content_comment_dto);

			auto content_uuid = boost::uuids::uuid{};
			try
			{
				content_uuid = co_await m_content_comments_contract->addContentCommentAsync(content_comment_entity);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Add content comment failed: " << e.what() << std::endl;
				throw; 
			}

			response.m_status_code = HttpStatusCode::Ok; 
			response.m_headers[HeaderKeys::CONTENT_TYPE] = "application/json";

			auto json_serialization_visitor = m_visitor_factory->createJsonSerializationVisitor(response.m_body);
			Dto::GuidDto guid_dto;
			guid_dto.m_uuid = content_uuid;

			guid_dto.accept(*validation_visitor);
			if (!validation_errors.empty())
			{
				throw std::runtime_error("Internal server error: guid created by server is invalid!");
			}

			guid_dto.accept(*json_serialization_visitor);

			std::cout << "Finished executing AddContentCommentCommand with success" << std::endl;

			co_return response;
		}
	}
}
