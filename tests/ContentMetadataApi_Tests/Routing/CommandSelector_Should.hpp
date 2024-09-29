#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../ContentMetadataApi/Routing/ICommandSelector.hpp"
#include "../../../ContentMetadataApi/Routing/CommandSelector.hpp"
#include "../Mocks/HttpCommandFactoriesFactoryMock.hpp"
#include "../Mocks/HttpCommandFactoryMock.hpp"

namespace ContentMetadataApi_Tests
{
    class CommandSelector_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::HttpCommandFactoriesFactoryMock>> m_mock_http_command_factories_factory;
        std::unique_ptr<::testing::NiceMock<Mocks::HttpCommandFactoryMock>> m_mock_http_command_factory;
        std::unique_ptr<ContentMetadataApi::Routing::ICommandSelector> m_sut;

        void SetUp() override
        {
            m_mock_http_command_factories_factory = std::make_shared<::testing::NiceMock<Mocks::HttpCommandFactoriesFactoryMock>>();
            m_mock_http_command_factory = std::make_unique<::testing::NiceMock<Mocks::HttpCommandFactoryMock>>();
            m_sut = std::make_unique<ContentMetadataApi::Routing::CommandSelector>(m_mock_http_command_factories_factory);
        }
    };
}
