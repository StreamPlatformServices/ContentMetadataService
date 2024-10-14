#pragma once
#include "../IHttpCommandFactory.hpp"
#include "../../../Mappers/EntityMappers/IContentCommentDataMapper.hpp"
#include "../../../Parsers/IGuidParser.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentCommentsContract.hpp"


namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT ContentCommentsCommandFactory : public IHttpCommandFactory
		{
		public:
			explicit ContentCommentsCommandFactory(
				std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract,
				std::shared_ptr<Mappers::IContentCommentDataMapper> a_content_comment_mapper,
				std::shared_ptr<Parsers::IGuidParser> a_guid_parser,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			ContentCommentsCommandFactory(const ContentCommentsCommandFactory&) = delete;
			ContentCommentsCommandFactory(ContentCommentsCommandFactory&&) = delete;
			
			ContentCommentsCommandFactory& operator=(const ContentCommentsCommandFactory&) = delete;
			ContentCommentsCommandFactory& operator=(ContentCommentsCommandFactory&&) = delete;

			auto createGetCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand>  override;
			auto createPostCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand>  override;
			auto createPutCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand>  override;
			auto createDeleteCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand>  override;
			auto createPatchCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand>  override;

		private:
			std::shared_ptr<ContentMetadataCore::IContentCommentsContract> m_content_comments_contract;
			std::shared_ptr<Mappers::IContentCommentDataMapper> m_content_comment_mapper;
			std::shared_ptr<Parsers::IGuidParser> m_guid_parser;
			std::shared_ptr<Visitors::IVisitorFactory>	m_visitor_factory;
		
		};
	}
}