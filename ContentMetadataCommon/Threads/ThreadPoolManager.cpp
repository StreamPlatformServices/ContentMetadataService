#include "ThreadPoolManager.hpp"

namespace ContentMetadataCommon
{
    ThreadPoolManager::ThreadPoolManager(unsigned int a_num_threads)
        : m_pool(a_num_threads)
    {
    }

    boost::asio::thread_pool& ThreadPoolManager::getThreadPool() noexcept
    {
        return m_pool;
    }

    void ThreadPoolManager::join()
    {
        m_pool.join();
    }
}