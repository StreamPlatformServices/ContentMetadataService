#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentCommentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class DeleteContentCommentCommand : public ICommand
		{

		public:
			explicit DeleteContentCommentCommand(
				const boost::uuids::uuid& a_content_comment_id,
				std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract) noexcept;

			DeleteContentCommentCommand(const DeleteContentCommentCommand&) = delete;
			DeleteContentCommentCommand(DeleteContentCommentCommand&&) = delete;

			DeleteContentCommentCommand& operator=(const DeleteContentCommentCommand&) = delete;
			DeleteContentCommentCommand& operator=(DeleteContentCommentCommand&&) = delete;

			boost::asio::awaitable<HttpResponse> executeAsync() const override;

		private:
			const boost::uuids::uuid m_content_comment_id;

			std::shared_ptr<ContentMetadataCore::IContentCommentsContract> m_content_comments_contract;
		};
	}
}
