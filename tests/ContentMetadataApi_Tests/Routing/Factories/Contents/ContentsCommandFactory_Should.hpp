#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../../ContentMetadataApi/Routing/Factories/Contents/ContentsCommandFactory.hpp"
#include "../../../Mocks/ContentContractMock.hpp"
#include "../../../Mocks/ContentDataMapperMock.hpp"
#include "../../../Mocks/VisitorFactoryMock.hpp"
#include "../../../Mocks/GuidParserMock.hpp"

namespace ContentMetadataApi_Tests
{

    class ContentsCommandFactory_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::ContentContractMock>> m_mock_contract;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentDataMapperMock>> m_mock_mapper;
        std::shared_ptr<::testing::NiceMock<Mocks::VisitorFactoryMock>> m_mock_visitor_factory;
        std::shared_ptr<::testing::NiceMock<Mocks::GuidParserMock>> m_mock_guid_parser;
        std::unique_ptr<ContentMetadataApi::Routing::IHttpCommandFactory> m_sut;

        void SetUp() override
        {
            m_mock_contract = std::make_shared<::testing::NiceMock<Mocks::ContentContractMock>>();
            m_mock_mapper = std::make_shared<::testing::NiceMock<Mocks::ContentDataMapperMock>>();
            m_mock_visitor_factory = std::make_shared<::testing::NiceMock<Mocks::VisitorFactoryMock>>();
            m_mock_guid_parser = std::make_shared<::testing::NiceMock<Mocks::GuidParserMock>>();

            m_sut = std::make_unique<ContentMetadataApi::Routing::ContentsCommandFactory>(
                m_mock_contract, m_mock_mapper, m_mock_guid_parser, m_mock_visitor_factory);
        }
    };
}