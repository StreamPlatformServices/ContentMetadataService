#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT AddContentCommand : public ICommand
		{

		public:
			AddContentCommand(
				const std::string& a_request_body,
				std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
				std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			AddContentCommand(const AddContentCommand&) = delete;
			AddContentCommand(AddContentCommand&&) = delete;

			AddContentCommand& operator=(const AddContentCommand&) = delete;
			AddContentCommand& operator=(AddContentCommand&&) = delete;


			boost::asio::awaitable<HttpResponse> executeAsync() const override;

		private:
			const std::string& m_request_body;

			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
			std::shared_ptr<Mappers::IContentDataMapper> m_content_mapper;
			std::shared_ptr<Visitors::IVisitorFactory>	m_visitor_factory;
		};
	}
}

