#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../ContentMetadataApi/Routing/Factories/HttpCommandFactoriesFactory.hpp"
#include "../../../../ContentMetadataApi/Routing/Factories/IHttpCommandFactoriesFactory.hpp"
#include "../../Mocks/ContentContractMock.hpp"
#include "../../Mocks/ContentCommentContractMock.hpp"
#include "../../Mocks/ContentDataMapperMock.hpp"
#include "../../Mocks/ContentCommentDataMapperMock.hpp"
#include "../../Mocks/GuidParserMock.hpp"
#include "../../Mocks/VisitorFactoryMock.hpp"

namespace ContentMetadataApi_Tests
{
    class HttpCommandFactoriesFactory_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::ContentContractMock>> m_mock_contents_contract;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentCommentContractMock>> m_mock_content_comments_contract;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentDataMapperMock>> m_mock_content_mapper;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentCommentDataMapperMock>> m_mock_content_comment_mapper;
        std::shared_ptr<::testing::NiceMock<Mocks::GuidParserMock>> m_mock_guid_parser;
        std::shared_ptr<::testing::NiceMock<Mocks::VisitorFactoryMock>> m_mock_visitor_factory;

        std::unique_ptr<ContentMetadataApi::Routing::IHttpCommandFactoriesFactory> m_sut;

        void SetUp() override
        {
            m_mock_contents_contract = std::make_shared<::testing::NiceMock<Mocks::ContentContractMock>>();
            m_mock_content_comments_contract = std::make_shared<::testing::NiceMock<Mocks::ContentCommentContractMock>>();
            m_mock_content_mapper = std::make_shared<::testing::NiceMock<Mocks::ContentDataMapperMock>>();
            m_mock_content_comment_mapper = std::make_shared<::testing::NiceMock<Mocks::ContentCommentDataMapperMock>>();
            m_mock_guid_parser = std::make_shared<::testing::NiceMock<Mocks::GuidParserMock>>();
            m_mock_visitor_factory = std::make_shared<::testing::NiceMock<Mocks::VisitorFactoryMock>>();

            m_sut = std::make_unique<ContentMetadataApi::Routing::HttpCommandFactoriesFactory>(
                m_mock_contents_contract, m_mock_content_comments_contract, m_mock_content_mapper,
                m_mock_content_comment_mapper, m_mock_guid_parser, m_mock_visitor_factory);
        }
    };
}
