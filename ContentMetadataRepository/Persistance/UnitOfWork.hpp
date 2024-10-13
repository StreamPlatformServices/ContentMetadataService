#pragma once
#include "IUnitOfWork.hpp"
#include "../../ContentMetadataCommon/Configuration/IConfig.hpp"
#include "../../ContentMetadataCommon/Threads/IThreadPoolManager.hpp"
#include "Repositories/IRepositoryFactory.hpp"

namespace ContentMetadataRepository
{
    class CONTENT_METADATA_REPOSITORY_EXPORT UnitOfWork : public IUnitOfWork
    {
    public:
        explicit UnitOfWork(
            std::shared_ptr<ContentMetadataCommon::IConfig> a_configuration,
            std::unique_ptr<IRepositoryFactory> a_repository_factory,
            std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager) noexcept;

        UnitOfWork(const UnitOfWork&) = delete;
        UnitOfWork(UnitOfWork&&) = delete;

        UnitOfWork& operator=(const UnitOfWork&) = delete;
        UnitOfWork& operator=(UnitOfWork&&) = delete;

        void initializeConnectionsStack(int a_connnections_count);

        auto commitTransactionAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<void> override;
        void rollbackTransaction(const boost::uuids::uuid& a_transaction_id) override;
        
        auto contentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<IContentRepository>> override;
        auto contentCommentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<IContentCommentRepository>> override;
        auto licenseRulesRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<ILicenseRulesRepository>> override;

    private:
        auto getTransactionAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::shared_ptr<pqxx::work>>;
        auto initializeTransactionAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::shared_ptr<pqxx::work>>;
        void clearTransaction(const boost::uuids::uuid& a_transaction_id);

    private:
        // Note: Containers store pointers (unique_ptr/shared_ptr), so the objects they point to are safe from being affected 
        // by operations on the containers themselves (such as insertions or deletions). Once a pointer (unique_ptr or shared_ptr) 
        // is extracted or passed to another location, it is safe to operate on the pointed object, even if the container undergoes 
        // modifications (like reallocation). Ensure proper synchronization when modifying the containers.

        std::shared_mutex m_connections_stack_mutex;
        std::vector<std::unique_ptr<pqxx::connection>> m_connections_stack;

        std::shared_mutex m_active_connections_mutex;
        std::unordered_map<boost::uuids::uuid, std::unique_ptr<pqxx::connection>> m_active_connections;

        std::shared_mutex m_transactions_shared_mutex;
        std::unordered_map<boost::uuids::uuid, std::shared_ptr<pqxx::work>> m_transactions;

        std::shared_ptr<ContentMetadataCommon::IConfig> m_configuration;
        std::unique_ptr<IRepositoryFactory> m_repository_factory;
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> m_thread_pool_manager;
    };

}