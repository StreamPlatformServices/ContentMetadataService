#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/GetContentByIdCommand.hpp"
#include "../../../Mocks/ContentContractMock.hpp"
#include "../../../Mocks/ContentDataMapperMock.hpp"
#include "../../../Mocks/VisitorFactoryMock.hpp"
#include "../../../Mocks/VisitorMock.hpp"

namespace ContentMetadataApi_Tests
{
    using DeserializationVisitorMock = Mocks::VisitorMock;
    using ValidationVisitorMock = Mocks::VisitorMock;
    using SerializationVisitorMock = Mocks::VisitorMock;

    class GetContentByIdCommand_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::ContentContractMock>> m_mock_contract;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentDataMapperMock>> m_mock_mapper;
        std::shared_ptr<::testing::NiceMock<Mocks::VisitorFactoryMock>> m_mock_visitor_factory;
        std::unique_ptr<::testing::NiceMock<ValidationVisitorMock>> m_mock_validation_visitor;
        std::unique_ptr<::testing::NiceMock<SerializationVisitorMock>> m_mock_serialization_visitor;
        std::unique_ptr<ContentMetadataApi::Routing::ICommand> m_sut;
        boost::uuids::uuid m_content_id;

        void SetUp() override
        {
            m_mock_contract = std::make_shared<::testing::NiceMock<Mocks::ContentContractMock>>();
            m_mock_mapper = std::make_shared<::testing::NiceMock<Mocks::ContentDataMapperMock>>();
            m_mock_visitor_factory = std::make_shared<::testing::NiceMock<Mocks::VisitorFactoryMock>>();

            m_mock_validation_visitor = std::make_unique<::testing::NiceMock<ValidationVisitorMock>>();
            m_mock_serialization_visitor = std::make_unique<::testing::NiceMock<SerializationVisitorMock>>();

            ON_CALL(*m_mock_visitor_factory, createDataValidationVisitor(testing::_))
                .WillByDefault([this](std::vector<std::string>&) {
                return std::move(m_mock_validation_visitor);
                    });

            ON_CALL(*m_mock_visitor_factory, createJsonSerializationVisitor(testing::_))
                .WillByDefault([this](std::string&) {
                return std::move(m_mock_serialization_visitor);
                    });

            m_content_id = boost::uuids::random_generator()();
            m_sut = std::make_unique<ContentMetadataApi::Routing::GetContentByIdCommand>(
                m_content_id, m_mock_contract, m_mock_mapper, m_mock_visitor_factory);
        }
    };



}
