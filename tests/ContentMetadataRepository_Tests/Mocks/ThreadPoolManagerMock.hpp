#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataCommon/Threads/IThreadPoolManager.hpp"

namespace ContentMetadataRepository_Tests
{
    namespace Mocks
    {
        class ThreadPoolManagerMock : public ContentMetadataCommon::IThreadPoolManager 
        {
        public:
            MOCK_METHOD(boost::asio::thread_pool&, getThreadPool, (), (override));
        };
    }
}