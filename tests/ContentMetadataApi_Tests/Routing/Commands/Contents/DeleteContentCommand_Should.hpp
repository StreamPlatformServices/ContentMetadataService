#pragma once
#include <gtest/gtest.h>
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/DeleteContentCommand.hpp"
#include "../../../Mocks/ContentContractMock.hpp"

namespace ContentMetadataApi_Tests
{
    class DeleteContentCommand_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::ContentContractMock>> m_mock_contract;
        std::unique_ptr<ContentMetadataApi::Routing::ICommand> m_sut;
        boost::uuids::uuid m_content_id;

        void SetUp() override
        {
            m_mock_contract = std::make_shared<::testing::NiceMock<Mocks::ContentContractMock>>();
            m_content_id = boost::uuids::random_generator()();
            m_sut = std::make_unique<ContentMetadataApi::Routing::DeleteContentCommand>(m_content_id, m_mock_contract);
        }
    };
}
