#pragma once
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>

namespace ContentMetadataCommon
{
    class IThreadPoolManager 
    {
    public:
        virtual ~IThreadPoolManager() = default;

        virtual boost::asio::thread_pool& getThreadPool() = 0;
    };
}

