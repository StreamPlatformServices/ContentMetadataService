#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataApi/Visitors/IVisitor.hpp"
#include "../../../ContentMetadataApi/Visitors/Factories/IVisitorFactory.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class VisitorFactoryMock : public ContentMetadataApi::Visitors::IVisitorFactory
        {
        public:
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Visitors::IVisitor>, createDataValidationVisitor, (std::vector<std::string>& a_errors_output), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Visitors::IVisitor>, createJsonDeserializationVisitor, (const std::string& a_json_string_input), (const, override));
            MOCK_METHOD(std::unique_ptr<ContentMetadataApi::Visitors::IVisitor>, createJsonSerializationVisitor, (std::string& a_json_string_output), (const, override));
        };
    }
}