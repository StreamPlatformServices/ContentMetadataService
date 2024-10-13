#include "AddContentCommand.hpp"
#include "../../../DataTransferObjects/GuidDto.hpp"
#include "../../../DataTransferObjects/ErrorsDto.hpp"
#include "../../../HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		AddContentCommand::AddContentCommand(
			const std::string& a_request_body,
			std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
			std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
			std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept
			: m_request_body(a_request_body)
			, m_contents_contract(a_contents_contract)
			, m_content_mapper(a_content_mapper)
			, m_visitor_factory(a_visitor_factory)
		{
		}

		boost::asio::awaitable<HttpResponse> AddContentCommand::executeAsync() const
		{
			std::cout << "Start executing AddContentCommand" << std::endl;

			HttpResponse response;
			std::vector<std::string> validation_errors;

			auto json_deserialization_visitor = m_visitor_factory->createJsonDeserializationVisitor(m_request_body);
			auto validation_visitor = m_visitor_factory->createDataValidationVisitor(validation_errors);
			

			Dto::ContentDto content_dto;
			content_dto.accept(*json_deserialization_visitor);
			content_dto.accept(*validation_visitor);

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

			auto content_entity = m_content_mapper->contentEntityFrom(content_dto);

			auto content_uuid = boost::uuids::uuid{};
			try
			{
				content_uuid = co_await m_contents_contract->addContentMetadataAsync(content_entity);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Add content metadata failed: " << e.what() << std::endl;
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

			std::cout << "Finished executing AddContentCommand with success" << std::endl;

			co_return response;
		}
	}
}
