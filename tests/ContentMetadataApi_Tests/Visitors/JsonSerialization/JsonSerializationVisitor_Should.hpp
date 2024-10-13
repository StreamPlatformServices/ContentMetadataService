#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../ContentMetadataApi/Visitors/IVisitor.hpp"
#include "../../../../ContentMetadataApi/Visitors/JsonSerialization/JsonSerializationVisitor.hpp"
#include "CommonMocks/DateTimeParserMock.hpp"
#include <rapidjson/document.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace ContentMetadataApi_Tests
{
    class JsonSerializationVisitor_Should : public ::testing::Test
    {
    protected:
        std::string m_json_output;
        std::shared_ptr<::testing::NiceMock<::Mocks::DateTimeParserMock>> m_mock_date_time_parser;
        std::unique_ptr<ContentMetadataApi::Visitors::IVisitor> m_sut;

        void SetUp() override
        {
            m_mock_date_time_parser = std::make_shared<::testing::NiceMock<Mocks::DateTimeParserMock>>();
            m_sut = std::make_unique<ContentMetadataApi::Visitors::JsonSerializationVisitor>(m_json_output, m_mock_date_time_parser);
        }

        void validateJsonDocument(const std::string& json_str)
        {
            rapidjson::Document doc;
            doc.Parse(json_str.c_str());
            ASSERT_FALSE(doc.HasParseError()) << "Failed to parse JSON!";
        }
    };

    class JsonSerializationVisitorContentDto_Should : public JsonSerializationVisitor_Should, public ::testing::WithParamInterface<std::tuple<
        std::string,
        std::string,
        std::string,
        int,        
        std::string,
        std::string,
        std::string,
        std::string>> 
    {
    };

    class JsonSerializationVisitorContentCommentDto_Should : public JsonSerializationVisitor_Should, public ::testing::WithParamInterface<std::tuple<
        std::string,  
        std::string,  
        std::string,
        std::string,  
        std::string>> 
    {
    };

    class JsonSerializationVisitorLicenseRulesDto_Should : public JsonSerializationVisitor_Should, public ::testing::WithParamInterface<std::tuple<
        std::string,  
        int,          
        ContentMetadataCore::Enums::LicenseType,
        ContentMetadataCore::Enums::LicenseDuration,
        bool>> 
    {
    };

    class JsonSerializationVisitorErrorsDto_Should : public JsonSerializationVisitor_Should, public ::testing::WithParamInterface<std::tuple<std::vector<std::string>>> 
    {
    };

    class JsonSerializationVisitorGuidDto_Should : public JsonSerializationVisitor_Should, public ::testing::WithParamInterface<std::tuple<std::string>> 
    {
    };

    class JsonSerializationVisitorContentsDto_Should : public JsonSerializationVisitor_Should, public ::testing::WithParamInterface<std::tuple<
        std::string, std::string, std::string, int, 
        std::string, std::string, std::string, std::string, 
        std::string, std::string, std::string, int,  
        std::string, std::string, std::string, std::string>>
    {
    };
}