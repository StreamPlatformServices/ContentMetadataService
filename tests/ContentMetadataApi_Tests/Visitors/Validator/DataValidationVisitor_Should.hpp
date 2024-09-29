#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../ContentMetadataApi/Visitors/IVisitor.hpp"
#include "../../../../ContentMetadataApi/Visitors/Validator/DataValidationVisitor.hpp"
#include "../../Mocks/GuidParserMock.hpp"

namespace ContentMetadataApi_Tests
{
    class DataValidationVisitor_Should : public ::testing::Test
    {
    protected:
        std::vector<std::string> m_errors;
        std::shared_ptr<::testing::NiceMock<Mocks::GuidParserMock>> m_mock_guid_parser;
        std::unique_ptr<ContentMetadataApi::Visitors::DataValidationVisitor> m_sut;

        void SetUp() override
        {
            m_mock_guid_parser = std::make_shared<::testing::NiceMock<ContentMetadataApi_Tests::Mocks::GuidParserMock>>();
            m_sut = std::make_unique<ContentMetadataApi::Visitors::DataValidationVisitor>(m_errors, m_mock_guid_parser);
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

    class DataValidationContentDtoVisitor_Should : public DataValidationVisitor_Should, public ::testing::WithParamInterface < std::tuple <
        std::string,
        std::string,
        int,        
        std::string,
        std::string,
        std::string,
        std::string,
        std::string,
        bool>>
    {
    };

    class DataValidationContentCommentDtoVisitor_Should : public DataValidationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        std::string, 
        std::string, 
        std::string, 
        std::string, 
        bool,
        bool>>
    {
    };

    class DataValidationLicenseRulesDtoVisitor_Should : public DataValidationVisitor_Should, public ::testing::WithParamInterface <std::tuple<
        int, 
        ContentMetadataCore::Enums::LicenseType, 
        ContentMetadataCore::Enums::LicenseDuration, 
        bool>>
    {
    };

    class DataValidationGuidDtoVisitor_Should : public DataValidationVisitor_Should, public ::testing::WithParamInterface<boost::uuids::uuid>
    {
    };

}