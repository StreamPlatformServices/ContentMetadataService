#pragma once
#include <gtest/gtest.h>
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/IContentDataMapper.hpp"
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/ContentDataMapper.hpp"
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/LicenseRulesDataMapper.hpp"
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/ContentCommentDataMapper.hpp"
#include "../../../../ContentMetadataCore/Entities/Content.hpp"
#include "../../../../ContentMetadataApi/DataTransferObjects/ContentDto.hpp"
#include <boost/uuid/string_generator.hpp>
#include <chrono>
#include <memory>

namespace ContentMetadataApi_Tests
{
    class ContentDataMapper_Should : public ::testing::TestWithParam<std::tuple<
        std::string,       // UUID
        std::string,       // Title
        int,               // Duration in hours (upload time)
        int,               // Video duration in seconds
        std::string,       // Description
        std::string,       // Owner UUID
        std::string,       // Video file UUID
        std::string        // Image file UUID
        >>
    {
    protected:
        std::shared_ptr<ContentMetadataApi::Mappers::IContentDataMapper> m_sut;
        std::shared_ptr<ContentMetadataApi::Mappers::IContentCommentDataMapper> m_content_comment_mapper;
        std::shared_ptr<ContentMetadataApi::Mappers::ILicenseRulesDataMapper> m_license_rules_mapper;

        ContentDataMapper_Should()
        {
            m_content_comment_mapper = std::make_shared<ContentMetadataApi::Mappers::ContentCommentDataMapper>();
            m_license_rules_mapper = std::make_shared<ContentMetadataApi::Mappers::LicenseRulesDataMapper>();
            m_sut = std::make_shared<ContentMetadataApi::Mappers::ContentDataMapper>(m_content_comment_mapper, m_license_rules_mapper);
        }
    };
}