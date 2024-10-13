#include "UnitOfWork.hpp"

namespace ContentMetadataRepository 
{
    UnitOfWork::UnitOfWork(
        std::shared_ptr<ContentMetadataCommon::IConfig> a_configuration,
        std::unique_ptr<IRepositoryFactory> a_repository_factory,
        std::shared_ptr<ContentMetadataCommon::IThreadPoolManager> a_thread_pool_manager) noexcept
        : m_configuration(a_configuration)
        , m_repository_factory(std::move(a_repository_factory))
        , m_thread_pool_manager(a_thread_pool_manager)
    {
    }

    void UnitOfWork::initializeConnectionsStack(int a_connnections_count)
    {
        //Note: Initialization is called on the start of application, before any new thread will be used from pool. (No race condition risk)
        while (a_connnections_count-- > 0) 
        {
            auto&& connection = std::make_unique<pqxx::connection>(std::format( 
                "dbname={} user={} password={}",
                m_configuration->getDbName(), m_configuration->getDbUsername(), m_configuration->getDbPassword()));

            m_connections_stack.push_back(std::move(connection));
        }
    }

    auto UnitOfWork::commitTransactionAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<void>
    {
        std::shared_ptr<pqxx::work> transaction;
        {
            std::shared_lock lock_transactions(m_transactions_shared_mutex);
            auto it = m_transactions.find(a_transaction_id);
            if (it == m_transactions.end())
            {
                std::cerr << "ERROR: Cannot commit transaction. Transaction with id " << a_transaction_id << " doesn't exist." << std::endl;
                throw std::runtime_error(std::format("ERROR: Cannot commit transaction. Transaction with id {} doesn't exist.", boost::uuids::to_string(a_transaction_id)));
            }

            transaction = it->second;
        }

        co_await boost::asio::post(m_thread_pool_manager->getThreadPool(), boost::asio::use_awaitable);

        try
        {
            transaction->commit();
            clearTransaction(a_transaction_id);
            std::cout << "Transaction " << a_transaction_id << " committed successfully." << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "ERROR: Failed to commit transaction with id " << a_transaction_id << ". Reason: " << e.what() << std::endl;
            throw;
        }
    }

    void UnitOfWork::rollbackTransaction(const boost::uuids::uuid& a_transaction_id)
    {
        std::shared_ptr<pqxx::work> transaction;
        {
            std::shared_lock lock_transactions(m_transactions_shared_mutex);
            auto it = m_transactions.find(a_transaction_id);
            if (it == m_transactions.end())
            {
                std::cerr << "ERROR: Cannot rollback transaction. Transaction with id " << a_transaction_id << " doesn't exist." << std::endl;
                throw std::runtime_error(std::format("ERROR: Cannot rollback transaction. Transaction with id {} doesn't exist.", boost::uuids::to_string(a_transaction_id)));
            }

            transaction = it->second;
        }

        try
        {   
            transaction->abort();
            
            clearTransaction(a_transaction_id);
            std::cout << "Transaction rolled back successfully." << std::endl;
        }
        catch (const std::exception& e)
        {
            clearTransaction(a_transaction_id);
            std::cerr << "ERROR: Failed to rollback transaction with id " << a_transaction_id << ". Reason: " << e.what() << std::endl;
            throw;
        }
    }

    auto UnitOfWork::contentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<IContentRepository>>
    {
        auto transaction = co_await getTransactionAsync(a_transaction_id);

        co_return m_repository_factory->createContentRepository(transaction);
    }

    auto UnitOfWork::contentCommentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<IContentCommentRepository>>
    {
        auto transaction = co_await getTransactionAsync(a_transaction_id);

        co_return m_repository_factory->createContentCommentRepository(transaction);
    }

    auto UnitOfWork::licenseRulesRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<ILicenseRulesRepository>>
    {
        auto transaction = co_await getTransactionAsync(a_transaction_id);

        co_return m_repository_factory->createLicenseRulesRepository(transaction);
    }

    auto UnitOfWork::getTransactionAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::shared_ptr<pqxx::work>>
    {
        {
            std::shared_lock lock_active_connections(m_active_connections_mutex);
            std::shared_lock lock_transactions(m_transactions_shared_mutex);

            auto connection = m_active_connections.find(a_transaction_id);
            auto transaction = m_transactions.find(a_transaction_id);

            if (connection == m_active_connections.end() ^ transaction == m_transactions.end())
            {
                std::cerr << "ERROR: Untracked transaction with existing connection or otherwise. Id: " << a_transaction_id << std::endl;
                throw std::runtime_error(std::format("ERROR: Untracked transaction with existing connection or otherwise. Id: {}", boost::uuids::to_string(a_transaction_id)));
            }

            if (connection != m_active_connections.end() || transaction != m_transactions.end())
            {
                co_return transaction->second;
            }
        }

        co_return co_await initializeTransactionAsync(a_transaction_id);
    }

    auto UnitOfWork::initializeTransactionAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::shared_ptr<pqxx::work>>
    {
        co_await boost::asio::post(m_thread_pool_manager->getThreadPool(), boost::asio::use_awaitable);

        pqxx::connection* connection;

        {
            std::unique_lock lock_connections_stack(m_connections_stack_mutex);
            std::unique_lock lock_active_connections(m_active_connections_mutex);

            while (m_connections_stack.empty())
            {
                // Note: The context switches rapidly between loop condition checks. This ensures that the thread remains available in the thread pool.
                // If finer control over the interval duration between checks is needed, consider using:
                // co_await m_timer.async_wait(boost::asio::use_awaitable)
                co_await boost::asio::this_coro::executor;
            }

            m_active_connections[a_transaction_id] = std::move(m_connections_stack.back());
            m_connections_stack.pop_back();

            auto it = m_active_connections.find(a_transaction_id);
            connection = &*it->second;
        }

        //Note: std::unordered_map is not thread safe. While any modification, there is a risk of data realocation. So this operation needs to be locked
        {
            std::unique_lock lock_transactions(m_transactions_shared_mutex);
            m_transactions[a_transaction_id] = std::make_shared<pqxx::work>(*connection);
        }

        std::shared_lock lock_transactions(m_transactions_shared_mutex);
        co_return m_transactions[a_transaction_id];
    }

    void UnitOfWork::clearTransaction(const boost::uuids::uuid& a_transaction_id)
    {
        std::unique_lock lock_connections_stack(m_connections_stack_mutex);
        std::unique_lock lock_active_connections(m_active_connections_mutex);
        std::unique_lock lock_transactions(m_transactions_shared_mutex);

        m_transactions.erase(a_transaction_id);
        auto it = m_active_connections.find(a_transaction_id);
        if (it == m_active_connections.end())
        {
            throw std::runtime_error("Could not release connection to stack. Connection doesn't exist.");
        }

        m_connections_stack.push_back(std::move(it->second));
        m_active_connections.erase(a_transaction_id);

    }

} // namespace ContentMetadataRepository

