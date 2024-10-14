#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentCommentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentCommentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT AddContentCommentCommand : public ICommand
		{

		public:
			explicit AddContentCommentCommand(
				const std::string& a_request_body,
				std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract,
				std::shared_ptr<Mappers::IContentCommentDataMapper> a_content_comment_mapper,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			AddContentCommentCommand(const AddContentCommentCommand&) = delete;
			AddContentCommentCommand(AddContentCommentCommand&&) = delete;

			AddContentCommentCommand& operator=(const AddContentCommentCommand&) = delete;
			AddContentCommentCommand& operator=(AddContentCommentCommand&&) = delete;

			auto executeAsync() const -> boost::asio::awaitable<HttpResponse> override;

		private:
			const std::string& m_request_body;

			std::shared_ptr<ContentMetadataCore::IContentCommentsContract> m_content_comments_contract;
			std::shared_ptr<Mappers::IContentCommentDataMapper> m_content_comment_mapper;
			std::shared_ptr<Visitors::IVisitorFactory>	m_visitor_factory;
		};
	}
}

