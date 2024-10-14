#include "GetContentByIdCommand.hpp"
#include "../../../DataTransferObjects/GuidDto.hpp"
#include "../../../DataTransferObjects/ErrorsDto.hpp"
#include "../../../HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		GetContentByIdCommand::GetContentByIdCommand(
			const boost::uuids::uuid& a_content_id,
			std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
			std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
			std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept
			: m_content_id(a_content_id)
			, m_contents_contract(a_contents_contract)
			, m_content_mapper(a_content_mapper)
			, m_visitor_factory(a_visitor_factory)
		{
		}

		auto GetContentByIdCommand::executeAsync() const -> boost::asio::awaitable<HttpResponse>
		{
			std::cout << "Start executing GetContentByIdCommand" << std::endl;
			HttpResponse response;
			std::vector<std::string> validation_errors;
			auto validation_visitor = m_visitor_factory->createDataValidationVisitor(validation_errors);
			
			Dto::ContentDto content_dto;
			try
			{
				auto content_entity = co_await m_contents_contract->getContentMetadataByIdAsync(m_content_id);
				content_dto = m_content_mapper->contentDtoFrom(content_entity);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Add content metadata failed: " << e.what() << std::endl;
				throw;
			}

			content_dto.accept(*validation_visitor);

			if (!validation_errors.empty())
			{
				std::cerr << "Recieved content data not valid: " << std::endl;
				for (const auto& error : validation_errors)
				{
					std::cerr << error << std::endl;
				}

				throw std::runtime_error("internal server error");
			}

			response.m_status_code = HttpStatusCode::Ok; 
			response.m_headers[HeaderKeys::CONTENT_TYPE] = "application/json";

			auto json_serialization_visitor = m_visitor_factory->createJsonSerializationVisitor(response.m_body);
			content_dto.accept(*json_serialization_visitor);

			std::cout << "Finish executing GetContentByIdCommand with success" << std::endl;

			co_return response;
		}
	}
}
