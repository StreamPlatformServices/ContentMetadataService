#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../ContentMetadataApi/Visitors/IVisitor.hpp"
#include "../../../../ContentMetadataApi/Visitors/Factories/IVisitorFactory.hpp"
#include "../../../../ContentMetadataApi/Visitors/Factories/VisitorFactory.hpp"
#include "../../../../ContentMetadataApi/Visitors/JsonSerialization/JsonDeserializationVisitor.hpp"
#include "../../../../ContentMetadataCore/Enums/LicenseType.hpp"
#include "../../../../ContentMetadataCore/Enums/LicenseDuration.hpp"
#include "CommonMocks/DateTimeParserMock.hpp"
#include "../../Mocks/GuidParserMock.hpp"

namespace ContentMetadataApi_Tests
{

    class JsonDeserializationVisitor_Should : public ::testing::Test
    {
    protected:
        std::shared_ptr<ContentMetadataApi::Visitors::IVisitorFactory> m_visitor_factory;
        std::shared_ptr<::testing::NiceMock<::Mocks::DateTimeParserMock>> m_mock_date_time_parser;
        std::shared_ptr<::testing::NiceMock<Mocks::GuidParserMock>> m_mock_guid_parser;

        void SetUp() override
        {
            m_mock_date_time_parser = std::make_shared<::testing::NiceMock<::Mocks::DateTimeParserMock>>();
            m_mock_guid_parser = std::make_shared<::testing::NiceMock<Mocks::GuidParserMock>>();
            m_visitor_factory = std::make_shared<ContentMetadataApi::Visitors::VisitorFactory>(m_mock_date_time_parser, m_mock_guid_parser);
        }

        std::chrono::system_clock::time_point stringToTimePoint(const std::string& date_time_str) {
            std::tm tm = {};
            std::stringstream ss(date_time_str);
            ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

            if (ss.fail()) {
                throw std::invalid_argument("Failed to parse time string: " + date_time_str);
            }

            std::time_t time = std::mktime(&tm);
            return std::chrono::system_clock::from_time_t(time);
        }

    };

    class JsonDeserializationVisitorContentDto_Should : public JsonDeserializationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        std::string, std::string, std::string, int,
        std::string, std::string, std::string, std::string>> 
    {  
    };

    class JsonDeserializationVisitorContentDtoInvalid_Should : public JsonDeserializationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        std::string, std::string, std::string, int,
        std::string, std::string, std::string, std::string>>
    {
    };

    class JsonDeserializationVisitorContentCommentDto_Should : public JsonDeserializationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        std::string, std::string, std::string, std::string, std::string>>
    {
    };

    class JsonDeserializationVisitorContentCommentDtoInvalid_Should : public JsonDeserializationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        std::string, std::string, std::string, std::string, std::string>>
    {
    };

    class JsonDeserializationVisitorLicenseRulesDto_Should : public JsonDeserializationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        std::string, int, ContentMetadataCore::Enums::LicenseType, 
        std::optional<ContentMetadataCore::Enums::LicenseDuration>>>  
    {
    };

    class JsonDeserializationVisitorLicenseRulesDtoInvalid_Should : public JsonDeserializationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        std::string, int, ContentMetadataCore::Enums::LicenseType,  
        std::optional<ContentMetadataCore::Enums::LicenseDuration>>>  
    {
    };

}