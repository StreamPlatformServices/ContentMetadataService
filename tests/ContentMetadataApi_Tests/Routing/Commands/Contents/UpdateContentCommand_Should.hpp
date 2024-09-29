#pragma once
#include <gtest/gtest.h>
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/UpdateContentCommand.hpp"
#include "../../../Mocks/ContentContractMock.hpp"
#include "../../../Mocks/ContentDataMapperMock.hpp"
#include "../../../Mocks/VisitorFactoryMock.hpp"
#include "../../../Mocks/VisitorMock.hpp"

namespace ContentMetadataApi_Tests
{
    using DeserializationVisitorMock = Mocks::VisitorMock;
    using ValidationVisitorMock = Mocks::VisitorMock;
    using SerializationVisitorMock = Mocks::VisitorMock;

    class UpdateContentCommand_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<::testing::NiceMock<Mocks::ContentContractMock>> m_mock_contract;
        std::shared_ptr<::testing::NiceMock<Mocks::ContentDataMapperMock>> m_mock_mapper;
        std::shared_ptr<::testing::NiceMock<Mocks::VisitorFactoryMock>> m_mock_visitor_factory;
        std::unique_ptr<::testing::NiceMock<DeserializationVisitorMock>> m_mock_deserialization_visitor;
        std::unique_ptr<::testing::NiceMock<ValidationVisitorMock>> m_mock_validation_visitor;
        std::unique_ptr<::testing::NiceMock<SerializationVisitorMock>> m_mock_serialization_visitor;

        boost::uuids::uuid m_content_id;
        std::string m_request_body;
        std::unique_ptr<ContentMetadataApi::Routing::ICommand> m_sut;

        void SetUp() override
        {
            m_mock_contract = std::make_shared<::testing::NiceMock<Mocks::ContentContractMock>>();
            m_mock_mapper = std::make_shared<::testing::NiceMock<Mocks::ContentDataMapperMock>>();
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

            m_content_id = boost::uuids::random_generator()();
            m_request_body = R"({
                "uuid": "11223344-5566-7788-99aa-bbccddeeff00",
                "title": "Updated content",
                "description": "Updated content description",
                "duration": 3600,
                "upload_time": "2023-09-25T10:15:30Z",
                "owner_id": "22334455-6677-8899-aabb-ccddeeff0011",
                "video_file_id": "33445566-7788-99aa-bbcc-ddeeff002233",
                "image_file_id": "44556677-8899-aabb-ccdd-eeff00334455"
            })";

            m_sut = std::make_unique<ContentMetadataApi::Routing::UpdateContentCommand>(
                m_content_id, m_request_body, m_mock_contract, m_mock_mapper, m_mock_visitor_factory);
        }
    };
}
