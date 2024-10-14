#pragma once
#include "IThreadPoolManager.hpp"
#include <boost/asio/thread_pool.hpp>
#include <thread>

namespace ContentMetadataCommon
{
    class ThreadPoolManager : public IThreadPoolManager
    {
    public:
        explicit ThreadPoolManager(unsigned int a_num_threads);

        auto getThreadPool() noexcept -> boost::asio::thread_pool& override;

        void join();

    private:
        boost::asio::thread_pool m_pool;
    };
}