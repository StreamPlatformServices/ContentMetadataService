#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT GetContentsByOwnerIdCommand : public ICommand
		{

		public:
			explicit GetContentsByOwnerIdCommand(
				const boost::uuids::uuid& a_content_id,
				std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
				std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			GetContentsByOwnerIdCommand(const GetContentsByOwnerIdCommand&) = delete;
			GetContentsByOwnerIdCommand(GetContentsByOwnerIdCommand&&) = delete;

			GetContentsByOwnerIdCommand& operator=(const GetContentsByOwnerIdCommand&) = delete;
			GetContentsByOwnerIdCommand& operator=(GetContentsByOwnerIdCommand&&) = delete;

			auto executeAsync() const -> boost::asio::awaitable<HttpResponse> override;

		private:
			const boost::uuids::uuid m_content_id;

			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
			std::shared_ptr<Mappers::IContentDataMapper> m_content_mapper;
			std::shared_ptr<Visitors::IVisitorFactory>	m_visitor_factory;
		};
	}
}

