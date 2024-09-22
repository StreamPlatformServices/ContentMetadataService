#include "DeleteContentCommand.hpp"
#include "../../../DataTransferObjects/GuidDto.hpp"
#include "../../../DataTransferObjects/ErrorsDto.hpp"
#include "../../../HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		DeleteContentCommand::DeleteContentCommand(
			const boost::uuids::uuid& a_content_id,
			std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract) noexcept
			: m_content_id(a_content_id)
			, m_contents_contract(a_contents_contract)
		{
		}

		boost::asio::awaitable<HttpResponse> DeleteContentCommand::executeAsync() const
		{
			std::cout << "Start executing DeleteContentCommand." << std::endl;
			HttpResponse response;

			try
			{
				co_await m_contents_contract->deleteContentMetadataAsync(m_content_id);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Add content metadata failed: " << e.what() << std::endl;
				throw; 
			}

			response.m_status_code = HttpStatusCode::Ok; 
			response.m_headers[HeaderKeys::CONTENT_TYPE] = "text/plain";

			response.m_body = "Content removed successfully.";

			std::cout << "Finish executing DeleteContentCommand with success." << std::endl;

			co_return response;
		}
	}
}
