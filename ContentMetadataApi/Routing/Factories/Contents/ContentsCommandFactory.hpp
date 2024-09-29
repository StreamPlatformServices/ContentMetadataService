#pragma once
#include "../IHttpCommandFactory.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Parsers/IGuidParser.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"


namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT ContentsCommandFactory : public IHttpCommandFactory
		{
		public:
			explicit ContentsCommandFactory(
				std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
				std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
				std::shared_ptr<Parsers::IGuidParser> a_guid_parser,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			ContentsCommandFactory(const ContentsCommandFactory&) = delete;
			ContentsCommandFactory(ContentsCommandFactory&&) = delete;

			ContentsCommandFactory& operator=(const ContentsCommandFactory&) = delete;
			ContentsCommandFactory& operator=(ContentsCommandFactory&&) = delete;
			
			std::unique_ptr<ICommand> createGetCommand(const HttpRequest& a_request) const override;
			std::unique_ptr<ICommand> createPostCommand(const HttpRequest& a_request) const override;
			std::unique_ptr<ICommand> createPutCommand(const HttpRequest& a_request) const override;
			std::unique_ptr<ICommand> createDeleteCommand(const HttpRequest& a_request) const override;
			std::unique_ptr<ICommand> createPatchCommand(const HttpRequest& a_request) const override;

		private:
			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
			std::shared_ptr<Mappers::IContentDataMapper> m_content_mapper;
			std::shared_ptr<Parsers::IGuidParser> m_guid_parser;
			std::shared_ptr<Visitors::IVisitorFactory> m_visitor_factory;
		};
	}
}