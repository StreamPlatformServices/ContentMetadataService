#pragma once
#include <gtest/gtest.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "../../../ContentMetadataApi/Parsers/IGuidParser.hpp"
#include "../../../ContentMetadataApi/Parsers/GuidParser.hpp"

namespace ContentMetadataApi_Tests
{
    class GuidParser_Should : public ::testing::Test 
    {
    protected:
        std::unique_ptr<ContentMetadataApi::Parsers::IGuidParser> m_sut = std::make_unique<ContentMetadataApi::Parsers::GuidParser>();
    };
}