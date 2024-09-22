#pragma once
#include "IHttpCommandFactoriesFactory.hpp"
#include "../../Visitors/Factories/IVisitorFactory.hpp"
#include "../../Parsers/IGuidParser.hpp"
#include "../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../Mappers/EntityMappers/ILicenseRulesDataMapper.hpp"
#include "../../Mappers/EntityMappers/IContentCommentDataMapper.hpp"
#include "../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "../../../ContentMetadataCore/InternalContracts/IContentCommentsContract.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT HttpCommandFactoriesFactory : public IHttpCommandFactoriesFactory
		{
		public:
			explicit HttpCommandFactoriesFactory(
				std::shared_ptr<ContentMetadataCore::IContentsContract>        a_contents_contract,
				std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract,
				std::shared_ptr<Mappers::IContentDataMapper>				   a_content_mapper,
				std::shared_ptr<Mappers::IContentCommentDataMapper>			   a_content_comment_mapper,
				std::shared_ptr<Parsers::IGuidParser>						   a_guid_parser,
				std::shared_ptr<Visitors::IVisitorFactory>					   a_visitor_factory) noexcept;

			HttpCommandFactoriesFactory(const HttpCommandFactoriesFactory&) = delete;
			HttpCommandFactoriesFactory(HttpCommandFactoriesFactory&&) = delete;

			HttpCommandFactoriesFactory& operator=(const HttpCommandFactoriesFactory&) = delete;
			HttpCommandFactoriesFactory& operator=(HttpCommandFactoriesFactory&&) = delete;

			std::unique_ptr<IHttpCommandFactory> createHttpCommandFactory(const std::string& a_root_endpoint) const override;

		private:
			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
			std::shared_ptr<ContentMetadataCore::IContentCommentsContract> m_content_comments_contract;

			std::shared_ptr<Mappers::IContentDataMapper> m_content_mapper;
			std::shared_ptr<Mappers::IContentCommentDataMapper> m_content_comment_mapper;
			std::shared_ptr<Parsers::IGuidParser> m_guid_parser;

			std::shared_ptr<Visitors::IVisitorFactory> m_visitor_factory;
		};
	}
}