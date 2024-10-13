#pragma once
#include "../ContentMetadataCore/InternalContracts/IContentsContract.hpp"
#include "Persistance/IUnitOfWork.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT ContentsDatabaseService : public ContentMetadataCore::IContentsContract
    {
    public:
        explicit ContentsDatabaseService(std::shared_ptr<IUnitOfWork> a_unit_of_work) noexcept;

        ContentsDatabaseService(const ContentsDatabaseService&) = delete;
        ContentsDatabaseService(ContentsDatabaseService&&) = delete;

        ContentsDatabaseService& operator=(const ContentsDatabaseService&) = delete;
        ContentsDatabaseService& operator=(ContentsDatabaseService&&) = delete;

        auto getAllContentsAsync(int a_limit, int a_offset) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> override;
        auto getContentMetadataByIdAsync(const boost::uuids::uuid& a_contentId) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content> override;
        auto getContentMetadataByOwnerIdAsync(const boost::uuids::uuid& a_ownerId) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>> override;
        auto editContentMetadataAsync(const boost::uuids::uuid& a_content_id, ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<void> override;
        auto addContentMetadataAsync(ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<boost::uuids::uuid> override;
        auto deleteContentMetadataAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<void> override;

    private:
        std::shared_ptr<IUnitOfWork> m_unit_of_work;
    };
}
