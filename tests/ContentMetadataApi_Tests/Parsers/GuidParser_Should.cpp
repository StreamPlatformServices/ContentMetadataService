#pragma once
#include "GuidParser_Should.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(GuidParser_Should, IsGuidWithValidUUID)
    {
        boost::uuids::random_generator gen;
        boost::uuids::uuid valid_uuid = gen();

        EXPECT_TRUE(m_sut->isGuid(valid_uuid));
    }


    TEST_F(GuidParser_Should, IsGuidWithNilUUID)
    {
        boost::uuids::uuid nil_uuid = boost::uuids::nil_uuid();

        EXPECT_FALSE(m_sut->isGuid(nil_uuid));
    }

    TEST_F(GuidParser_Should, IsGuidWithValidString)
    {
        std::string valid_guid_str = "01234567-89ab-cdef-0123-456789abcdef";

        EXPECT_TRUE(m_sut->isGuid(valid_guid_str));
    }


    TEST_F(GuidParser_Should, IsGuidWithInvalidString)
    {
        std::string invalid_guid_str = "invalid-guid";

        EXPECT_FALSE(m_sut->isGuid(invalid_guid_str));
    }

    TEST_F(GuidParser_Should, IsGuidWithEmptyString)
    {
        std::string empty_guid_str = "";

        EXPECT_FALSE(m_sut->isGuid(empty_guid_str));
    }


    TEST_F(GuidParser_Should, ParseGuidWithValidString)
    {
        std::string valid_guid_str = "01234567-89ab-cdef-0123-456789abcdef";

        boost::uuids::uuid parsed_uuid = m_sut->parseGuid(valid_guid_str);

        EXPECT_EQ(boost::uuids::to_string(parsed_uuid), valid_guid_str);
    }


    TEST_F(GuidParser_Should, ParseGuidWithInvalidStringThrows)
    {
        std::string invalid_guid_str = "invalid-guid";

        EXPECT_THROW(m_sut->parseGuid(invalid_guid_str), std::exception);
    }


    TEST_F(GuidParser_Should, ParseGuidWithEmptyStringThrows)
    {
        std::string empty_guid_str = "";

        EXPECT_THROW(m_sut->parseGuid(empty_guid_str), std::exception);
    }
}