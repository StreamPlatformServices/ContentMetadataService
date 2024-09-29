#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../../ContentMetadataApi/Routing/Factories/ContentComments/ContentCommentsCommandFactory.hpp"
#include "../../../Mocks/ContentCommentContractMock.hpp"
#include "../../../Mocks/ContentCommentDataMapperMock.hpp"
#include "../../../Mocks/GuidParserMock.hpp"
#include "../../../Mocks/VisitorFactoryMock.hpp"

namespace ContentMetadataApi_Tests
{
    class ContentCommentsCommandFactory_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::ContentCommentContractMock>> m_mock_contract;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentCommentDataMapperMock>> m_mock_mapper;
        std::shared_ptr<::testing::NiceMock<Mocks::GuidParserMock>> m_mock_guid_parser;
        std::shared_ptr<::testing::NiceMock<Mocks::VisitorFactoryMock>> m_mock_visitor_factory;
        std::unique_ptr<ContentMetadataApi::Routing::IHttpCommandFactory> m_sut;

        void SetUp() override
        {
            m_mock_contract = std::make_shared<::testing::NiceMock<Mocks::ContentCommentContractMock>>();
            m_mock_mapper = std::make_shared<::testing::NiceMock<Mocks::ContentCommentDataMapperMock>>();
            m_mock_guid_parser = std::make_shared<::testing::NiceMock<Mocks::GuidParserMock>>();
            m_mock_visitor_factory = std::make_shared<::testing::NiceMock<Mocks::VisitorFactoryMock>>();

            m_sut = std::make_unique<ContentMetadataApi::Routing::ContentCommentsCommandFactory>(
                m_mock_contract, m_mock_mapper, m_mock_guid_parser, m_mock_visitor_factory);
        }
    };
}
