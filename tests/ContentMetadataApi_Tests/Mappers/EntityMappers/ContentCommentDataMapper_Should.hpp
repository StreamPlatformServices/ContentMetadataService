#pragma once
#include <gtest/gtest.h>
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/IContentCommentDataMapper.hpp"
#include "../../../../ContentMetadataApi/Mappers/EntityMappers/ContentCommentDataMapper.hpp"
#include "../../../../ContentMetadataCore/Entities/ContentComment.hpp"
#include "../../../../ContentMetadataApi/DataTransferObjects/ContentCommentDto.hpp"

class ContentCommentDataMapper_Should : public ::testing::TestWithParam<std::tuple<
    std::string, std::string, int, std::string, std::string>>
{
protected:
    std::unique_ptr<ContentMetadataApi::Mappers::IContentCommentDataMapper> m_sut = std::make_unique<ContentMetadataApi::Mappers::ContentCommentDataMapper>();
    
    bool compareTimePoints(const std::chrono::system_clock::time_point& t1, const std::chrono::system_clock::time_point& t2) const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(t1.time_since_epoch()) == std::chrono::duration_cast<std::chrono::seconds>(t2.time_since_epoch());
    }
};
