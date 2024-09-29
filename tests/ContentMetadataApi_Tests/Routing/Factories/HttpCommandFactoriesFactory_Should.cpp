#include "HttpCommandFactoriesFactory_Should.hpp"
#include "../../../../ContentMetadataApi/Routing/Factories/ContentComments/ContentCommentsCommandFactory.hpp"
#include "../../../../ContentMetadataApi/Routing/Factories/Contents/ContentsCommandFactory.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(HttpCommandFactoriesFactory_Should, ReturnContentCommentsCommandFactoryForCommentEndpoint)
    {
        const std::string endpoint = "comment";
        auto factory = m_sut->createHttpCommandFactory(endpoint);

        ASSERT_NE(factory, nullptr);
        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::ContentCommentsCommandFactory*>(factory.get()), nullptr);
    }

    TEST_F(HttpCommandFactoriesFactory_Should, ReturnContentsCommandFactoryForContentEndpoint)
    {
        const std::string endpoint = "content";
        auto factory = m_sut->createHttpCommandFactory(endpoint);

        ASSERT_NE(factory, nullptr);
        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::ContentsCommandFactory*>(factory.get()), nullptr);
    }

    TEST_F(HttpCommandFactoriesFactory_Should, ReturnNullptrForUnknownEndpoint)
    {
        const std::string unknown_endpoint = "unknown";
        auto factory = m_sut->createHttpCommandFactory(unknown_endpoint);

        EXPECT_EQ(factory, nullptr);
    }

}