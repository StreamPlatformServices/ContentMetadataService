#pragma once
#include <gtest/gtest.h>
#include "../../../../../ContentMetadataApi/Routing/Commands/ContentComments/DeleteContentCommentCommand.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/ICommand.hpp"
#include "../../../Mocks/ContentCommentContractMock.hpp"

namespace ContentMetadataApi_Tests
{
    class DeleteContentCommentCommand_Should : public ::testing::Test
    {
    protected:
        std::unique_ptr<ContentMetadataApi::Routing::ICommand> m_sut;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentCommentContractMock>> m_mock_contract;
        boost::uuids::uuid m_content_comment_id;

        void SetUp() override
        {
            m_mock_contract = std::make_shared<::testing::NiceMock<Mocks::ContentCommentContractMock>>();
            m_content_comment_id = boost::uuids::random_generator()();
            m_sut = std::make_unique<ContentMetadataApi::Routing::DeleteContentCommentCommand>(m_content_comment_id, m_mock_contract);
        }
    };
}

