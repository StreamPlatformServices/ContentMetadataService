#include "../ContentMetadataCore/Entities/Content.hpp"
#include "ContentsDatabaseService.hpp"

namespace ContentMetadataRepository
{
    ContentsDatabaseService::ContentsDatabaseService(std::shared_ptr<IUnitOfWork> a_unit_of_work) noexcept
        : m_unit_of_work(a_unit_of_work)
    {
    }

    auto ContentsDatabaseService::getAllContentsAsync(int a_limit, int a_offset) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto content_repository = co_await m_unit_of_work->contentRepositoryAsync(transaction_id);
        auto license_rules_repository = co_await m_unit_of_work->licenseRulesRepositoryAsync(transaction_id);
        auto content_comment_repository = co_await m_unit_of_work->contentCommentRepositoryAsync(transaction_id);

        try
        {
            auto content_entities = co_await content_repository->getAllAsync(a_limit, a_offset);

            for (auto& content : content_entities)
            {
                content.m_license_rules = co_await license_rules_repository->getByContentIdAsync(content.m_uuid);
                content.m_content_comments = co_await content_comment_repository->getByContentIdAsync(content.m_uuid);
            }

            co_await m_unit_of_work->commitTransactionAsync(transaction_id);
            co_return content_entities;
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }

    auto ContentsDatabaseService::getContentMetadataByIdAsync(const boost::uuids::uuid& a_content_id) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content>
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto content_repository = co_await m_unit_of_work->contentRepositoryAsync(transaction_id);
        auto license_rules_repository = co_await m_unit_of_work->licenseRulesRepositoryAsync(transaction_id);
        auto content_comment_repository = co_await m_unit_of_work->contentCommentRepositoryAsync(transaction_id);

        try
        {
            auto content_entity = co_await content_repository->getByIdAsync(a_content_id);
            content_entity.m_license_rules = co_await license_rules_repository->getByContentIdAsync(content_entity.m_uuid);
            content_entity.m_content_comments = co_await content_comment_repository->getByContentIdAsync(content_entity.m_uuid);

            co_await m_unit_of_work->commitTransactionAsync(transaction_id);
            co_return content_entity;
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }

    auto ContentsDatabaseService::getContentMetadataByOwnerIdAsync(const boost::uuids::uuid& a_owner_id) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto content_repository = co_await m_unit_of_work->contentRepositoryAsync(transaction_id);
        auto license_rules_repository = co_await m_unit_of_work->licenseRulesRepositoryAsync(transaction_id);

        try
        {
            auto content_entities = co_await content_repository->getByOwnerIdAsync(a_owner_id);

            for (auto& content : content_entities)
            {
                content.m_license_rules = co_await license_rules_repository->getByContentIdAsync(content.m_uuid);
            }

            co_await m_unit_of_work->commitTransactionAsync(transaction_id);
            co_return content_entities;
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }

    auto ContentsDatabaseService::editContentMetadataAsync(const boost::uuids::uuid& a_content_id, ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<void>
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto content_repository = co_await m_unit_of_work->contentRepositoryAsync(transaction_id);
        auto license_rules_repository = co_await m_unit_of_work->licenseRulesRepositoryAsync(transaction_id);
        a_content.m_uuid = a_content_id;

        try
        {
            co_await license_rules_repository->deleteByContentIdAsync(a_content_id);

            co_await content_repository->updateAsync(a_content);

            for (auto& license_rule : a_content.m_license_rules)
            {
                co_await license_rules_repository->addAsync(license_rule);
            }

            co_await m_unit_of_work->commitTransactionAsync(transaction_id);
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }

    auto ContentsDatabaseService::addContentMetadataAsync(ContentMetadataCore::Entities::Content& a_content) -> boost::asio::awaitable<boost::uuids::uuid>
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto content_repository = co_await m_unit_of_work->contentRepositoryAsync(transaction_id);
        auto license_rules_repository = co_await m_unit_of_work->licenseRulesRepositoryAsync(transaction_id);

        try
        {
            auto content_id = co_await content_repository->addAsync(a_content);

            for (auto& license_rule : a_content.m_license_rules)
            {
                co_await license_rules_repository->addAsync(license_rule);
            }

            co_await m_unit_of_work->commitTransactionAsync(transaction_id);

            co_return content_id;
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }

    boost::asio::awaitable<void> ContentsDatabaseService::deleteContentMetadataAsync(const boost::uuids::uuid& a_content_id)
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto content_repository = co_await m_unit_of_work->contentRepositoryAsync(transaction_id);

        try
        {
            co_await content_repository->deleteAsync(a_content_id);
            co_await m_unit_of_work->commitTransactionAsync(transaction_id);
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }
} // namespace ContentMetadataRepository
