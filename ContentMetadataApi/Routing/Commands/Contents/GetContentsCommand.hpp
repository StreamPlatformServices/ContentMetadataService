#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT GetContentsCommand : public ICommand
		{

		public:
			explicit GetContentsCommand(
				const std::unordered_map<std::string, std::string>& a_query_params,
				std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
				std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
				std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept;

			GetContentsCommand(const GetContentsCommand&) = delete;
			GetContentsCommand(GetContentsCommand&&) = delete;

			GetContentsCommand& operator=(const GetContentsCommand&) = delete;
			GetContentsCommand& operator=(GetContentsCommand&&) = delete;

			auto executeAsync() const -> boost::asio::awaitable<HttpResponse>  override;

		private:
			bool getValueFromQueryParam(const std::string& a_key, int& a_value_out) const;

		private:
			const std::unordered_map<std::string, std::string>& m_query_params;

			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
			std::shared_ptr<Mappers::IContentDataMapper> m_content_mapper;
			std::shared_ptr<Visitors::IVisitorFactory>	m_visitor_factory;
		};
	}
}

