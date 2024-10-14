#include "DeleteContentCommentCommand.hpp"
#include "../../../DataTransferObjects/GuidDto.hpp"
#include "../../../DataTransferObjects/ErrorsDto.hpp"
#include "../../../HttpDataStructures/HeaderKeys.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		DeleteContentCommentCommand::DeleteContentCommentCommand(
			const boost::uuids::uuid& a_content_comment_id,
			std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract) noexcept
			: m_content_comment_id(a_content_comment_id)
			, m_content_comments_contract(a_content_comments_contract)
		{
		}

		auto DeleteContentCommentCommand::executeAsync() const -> boost::asio::awaitable<HttpResponse>
		{
			std::cout << "Start executing DeleteContentCommentCommand." << std::endl;
			HttpResponse response;

			try
			{
				co_await m_content_comments_contract->deleteContentCommentAsync(m_content_comment_id);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Add content comment failed: " << e.what() << std::endl;
				throw; 
			}

			response.m_status_code = HttpStatusCode::Ok; 
			response.m_headers[HeaderKeys::CONTENT_TYPE] = "text/plain";

			response.m_body = "Content comment removed successfully.";

			std::cout << "Finish executing DeleteContentCommentCommand with success." << std::endl;

			co_return response;
		}
	}
}
