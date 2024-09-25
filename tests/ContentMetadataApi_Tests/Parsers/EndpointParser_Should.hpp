#pragma once
#include <gtest/gtest.h>
#include "../../../ContentMetadataApi/Parsers/EndpointParser.hpp"

class EndpointParser_Should : public ::testing::Test 
{

protected:
   std::unique_ptr<ContentMetadataApi::Parsers::IEndpointParser> m_sut = std::make_unique<ContentMetadataApi::Parsers::EndpointParser>();

};

