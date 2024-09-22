#include "GetContentsCommand.hpp"
#include "../../../DataTransferObjects/ContentsDto.hpp"
#include "../../../DataTransferObjects/GuidDto.hpp"
#include "../../../DataTransferObjects/ErrorsDto.hpp"
#include "../../../HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		namespace
		{
			constexpr auto LIMIT_QUERY_PARAM_KEY = "limit";
			constexpr auto OFFSET_QUERY_PARAM_KEY = "offset";
		}

		GetContentsCommand::GetContentsCommand(
			const std::unordered_map<std::string, std::string>& a_query_params,
			std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
			std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
			std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept
			: m_query_params(a_query_params)
			, m_contents_contract(a_contents_contract)
			, m_content_mapper(a_content_mapper)
			, m_visitor_factory(a_visitor_factory)
		{
		}

		boost::asio::awaitable<HttpResponse> GetContentsCommand::executeAsync() const
		{
			std::cout << "Start executing GetContentsCommand" << std::endl;
			HttpResponse response;
			std::vector<std::string> validation_errors;

			auto validation_visitor = m_visitor_factory->createDataValidationVisitor(validation_errors);

			int limit = 0;
			int offset = 0;

			if (!getValueFromQueryParam(LIMIT_QUERY_PARAM_KEY, limit))
			{
				validation_errors.push_back("Query parameter limit invalid.");
			}

			if (!getValueFromQueryParam(OFFSET_QUERY_PARAM_KEY, offset))
			{
				validation_errors.push_back("Query parameter offset invalid.");
			}

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

			Dto::ContentsDto contents_dto;
			try
			{
				auto content_entities = co_await m_contents_contract->getAllContentsAsync(limit, offset);
				for (const auto& content_entity : content_entities)
				{
					auto&& content_dto = m_content_mapper->contentDtoFrom(content_entity);
					contents_dto.m_contents.push_back(content_dto);
				}
				
			}
			catch (const std::exception& e)
			{
				std::cerr << "Add content metadata failed: " << e.what() << std::endl;
				throw; 
			}

			contents_dto.accept(*validation_visitor);

			if (!validation_errors.empty())
			{
				std::cerr << "Recieved content data not valid: " << std::endl;
				for (const auto& error : validation_errors)
				{
					std::cerr << error << std::endl;
				}

				throw;
			}

			response.m_status_code = HttpStatusCode::Ok; 
			response.m_headers[HeaderKeys::CONTENT_TYPE] = "application/json";

			auto json_serialization_visitor = m_visitor_factory->createJsonSerializationVisitor(response.m_body);
			contents_dto.accept(*json_serialization_visitor);

			std::cout << "Finish executing GetContentsCommand with success" << std::endl;

			co_return response;
		}

		bool GetContentsCommand::getValueFromQueryParam(const std::string& a_key, int& a_value_out) const
		{
			auto it = m_query_params.find(a_key);
			if (it != m_query_params.end())
			{
				auto [ptr, ec] = std::from_chars(it->second.data(), it->second.data() + it->second.size(), a_value_out);
				if (ec == std::errc())
				{
					return true;
				}
			}

			return false;
		}
	}
}
