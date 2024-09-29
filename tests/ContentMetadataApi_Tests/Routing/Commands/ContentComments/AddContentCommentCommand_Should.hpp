#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../../ContentMetadataApi/Routing/Commands/ContentComments/AddContentCommentCommand.hpp"
#include "../../../Mocks/ContentCommentContractMock.hpp"
#include "../../../Mocks/ContentCommentDataMapperMock.hpp"
#include "../../../Mocks/VisitorFactoryMock.hpp"
#include "../../../Mocks/VisitorMock.hpp"


namespace ContentMetadataApi_Tests
{
    using DeserializationVisitorMock = Mocks::VisitorMock;
    using ValidationVisitorMock = Mocks::VisitorMock;
    using SerializationVisitorMock = Mocks::VisitorMock;

    class AddContentCommentCommand_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::ContentCommentContractMock>> m_mock_contract;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentCommentDataMapperMock>> m_mock_mapper;
        std::shared_ptr<::testing::NiceMock<Mocks::VisitorFactoryMock>> m_mock_visitor_factory;
        std::unique_ptr<::testing::NiceMock<DeserializationVisitorMock>> m_mock_deserialization_visitor;
        std::unique_ptr<::testing::NiceMock<ValidationVisitorMock>> m_mock_validation_visitor;
        std::unique_ptr<::testing::NiceMock<SerializationVisitorMock>> m_mock_serialization_visitor;
        std::string m_request_body;
        std::unique_ptr<ContentMetadataApi::Routing::ICommand> m_sut;

        void SetUp() override
        {
            m_mock_contract = std::make_shared<::testing::NiceMock<Mocks::ContentCommentContractMock>>();
            m_mock_mapper = std::make_shared<::testing::NiceMock<Mocks::ContentCommentDataMapperMock>>();
            m_mock_visitor_factory = std::make_shared<::testing::NiceMock<Mocks::VisitorFactoryMock>>();

            m_mock_deserialization_visitor = std::make_unique<::testing::NiceMock<DeserializationVisitorMock>>();
            m_mock_validation_visitor = std::make_unique<::testing::NiceMock<ValidationVisitorMock>>();
            m_mock_serialization_visitor = std::make_unique<::testing::NiceMock<SerializationVisitorMock>>();

            ON_CALL(*m_mock_visitor_factory, createJsonDeserializationVisitor(testing::_))
                .WillByDefault([this](const std::string&) {
                return std::move(m_mock_deserialization_visitor);
                    });

            ON_CALL(*m_mock_visitor_factory, createDataValidationVisitor(testing::_))
                .WillByDefault([this](std::vector<std::string>&) {
                return std::move(m_mock_validation_visitor);
                    });

            ON_CALL(*m_mock_visitor_factory, createJsonSerializationVisitor(testing::_))
                .WillByDefault([this](std::string&) {
                return std::move(m_mock_serialization_visitor);
                    });

            m_request_body = R"({
                "uuid": "11223344-5566-7788-99aa-bbccddeeff00",
                "body": "Sample content comment",
                "user_name": "Sample User",
                "creation_time": "2023-09-25T10:15:30Z",
                "content_id": "22334455-6677-8899-aabb-ccddeeff0011"
            })";

            m_sut = std::make_unique<ContentMetadataApi::Routing::AddContentCommentCommand>(
                m_request_body, m_mock_contract, m_mock_mapper, m_mock_visitor_factory);
        }
    };
}

