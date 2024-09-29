#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT GetContentByIdCommand : public ICommand
		{

		public:
			explicit GetContentByIdCommand(
				const boost::uuids::uuid& a_content_id,
				std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
				std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			GetContentByIdCommand(const GetContentByIdCommand&) = delete;
			GetContentByIdCommand(GetContentByIdCommand&&) = delete;

			GetContentByIdCommand& operator=(const GetContentByIdCommand&) = delete;
			GetContentByIdCommand& operator=(GetContentByIdCommand&&) = delete;

			boost::asio::awaitable<HttpResponse> executeAsync() const override;

		private:
			const boost::uuids::uuid m_content_id;

			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
			std::shared_ptr<Mappers::IContentDataMapper> m_content_mapper;
			std::shared_ptr<Visitors::IVisitorFactory>	m_visitor_factory;
		};
	}
}

