#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class UpdateContentCommand : public ICommand
		{

		public:
			explicit UpdateContentCommand(
				const boost::uuids::uuid& a_content_id,
				const std::string& a_request_body,
				std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
				std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			UpdateContentCommand(const UpdateContentCommand&) = delete;
			UpdateContentCommand(UpdateContentCommand&&) = delete;

			UpdateContentCommand& operator=(const UpdateContentCommand&) = delete;
			UpdateContentCommand& operator=(UpdateContentCommand&&) = delete;

			boost::asio::awaitable<HttpResponse> executeAsync() const override;

		private:
			const boost::uuids::uuid& m_content_id;
			const std::string& m_request_body;

			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
			std::shared_ptr<Mappers::IContentDataMapper> m_content_mapper;
			std::shared_ptr<Visitors::IVisitorFactory>	m_visitor_factory;
		};
	}
}

