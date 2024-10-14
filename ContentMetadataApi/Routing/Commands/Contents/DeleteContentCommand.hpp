#pragma once
#include "../ICommand.hpp"
#include "../../../../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "../../../Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class CONTENT_METADATA_API_EXPORT DeleteContentCommand : public ICommand
		{

		public:
			explicit DeleteContentCommand(
				const boost::uuids::uuid& a_content_id,
				std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract) noexcept;

			DeleteContentCommand(const DeleteContentCommand&) = delete;
			DeleteContentCommand(DeleteContentCommand&&) = delete;

			DeleteContentCommand& operator=(const DeleteContentCommand&) = delete;
			DeleteContentCommand& operator=(DeleteContentCommand&&) = delete;

			auto executeAsync() const -> boost::asio::awaitable<HttpResponse>  override;

		private:
			const boost::uuids::uuid m_content_id;

			std::shared_ptr<ContentMetadataCore::IContentsContract> m_contents_contract;
		};
	}
}

