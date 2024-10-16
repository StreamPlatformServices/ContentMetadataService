#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Mocks/RepositoryFactoryMock.hpp"
#include "../Mocks/ContentRepositoryMock.hpp"
#include "../Mocks/ContentCommentRepositoryMock.hpp"
#include "../Mocks/LicenseRulesRepositoryMock.hpp"
#include "../Mocks/ThreadPoolManagerMock.hpp"
#include "../../../ContentMetadataRepository/Persistance/UnitOfWork.hpp"
#include "../../../ContentMetadataRepository/Persistance/IUnitOfWork.hpp"
#include "../../../ContentMetadataCommon/Configuration/Config.hpp"
#include "../../../ContentMetadataRepository/Constants/DatabaseKeys.hpp"

namespace ContentMetadataRepository_Tests
{
    class UnitOfWork_Should : public ::testing::Test
    {
    protected:
        boost::asio::thread_pool m_thread_pool{ 4 };
        std::shared_ptr<::testing::NiceMock<Mocks::ThreadPoolManagerMock>> m_thread_pool_manager_mock;
        ::testing::NiceMock<Mocks::RepositoryFactoryMock>* m_repository_factory_mock;
        std::unique_ptr<ContentMetadataRepository::IUnitOfWork> m_sut;
        boost::uuids::uuid m_test_transaction_id;

        std::shared_ptr<ContentMetadataCommon::Config> m_config;

        void SetUp() override
        {
            m_config = std::make_shared<ContentMetadataCommon::Config>("config.ini");
            m_test_transaction_id = boost::uuids::random_generator()();
            m_thread_pool_manager_mock = std::make_shared<::testing::NiceMock<Mocks::ThreadPoolManagerMock>>();
            m_repository_factory_mock = new ::testing::NiceMock<Mocks::RepositoryFactoryMock>();

            auto uow = std::make_unique<ContentMetadataRepository::UnitOfWork>(
                m_config,
                std::unique_ptr<::testing::NiceMock<Mocks::RepositoryFactoryMock>>(m_repository_factory_mock),
                m_thread_pool_manager_mock
            );

            uow->initializeConnectionsStack(3);

            m_sut = std::move(uow);
        }

        void TearDown() override
        {
            m_thread_pool.join(); 
        }
    };
}
