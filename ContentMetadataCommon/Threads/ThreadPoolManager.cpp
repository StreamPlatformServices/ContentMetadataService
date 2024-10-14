#include "ThreadPoolManager.hpp"

namespace ContentMetadataCommon
{
    ThreadPoolManager::ThreadPoolManager(unsigned int a_num_threads)
        : m_pool(a_num_threads)
    {
    }

    auto ThreadPoolManager::getThreadPool() noexcept -> boost::asio::thread_pool&
    {
        return m_pool;
    }

    void ThreadPoolManager::join()
    {
        m_pool.join();
    }
}