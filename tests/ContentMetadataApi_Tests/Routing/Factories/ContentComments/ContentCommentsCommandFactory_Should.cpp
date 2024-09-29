#include "ContentCommentsCommandFactory_Should.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/ContentComments/AddContentCommentCommand.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/ContentComments/DeleteContentCommentCommand.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(ContentCommentsCommandFactory_Should, CreatePostCommandForAddingComment)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_body = R"({"user_name": "TestUser", "body": "Test comment"})";

        auto command = m_sut->createPostCommand(request);

        ASSERT_NE(command, nullptr);
        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::AddContentCommentCommand*>(command.get()), nullptr);
    }

    TEST_F(ContentCommentsCommandFactory_Should, ReturnNullptrForPostWithInvalidRoutingPath)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_body = R"({"user_name": "TestUser", "body": "Test comment"})";
        request.m_routing_path = { "invalidPath" };

        auto command = m_sut->createPostCommand(request);

        EXPECT_EQ(command, nullptr);
    }

    TEST_F(ContentCommentsCommandFactory_Should, CreateDeleteCommandForValidContentCommentId)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "11223344-5566-7788-99aa-bbccddeeff00" };

        EXPECT_CALL(*m_mock_guid_parser, isGuid(request.m_routing_path.at(0)))
            .WillOnce(::testing::Return(true));

        EXPECT_CALL(*m_mock_guid_parser, parseGuid(request.m_routing_path.at(0)))
            .WillOnce(::testing::Return(boost::uuids::string_generator()("11223344-5566-7788-99aa-bbccddeeff00")));

        auto command = m_sut->createDeleteCommand(request);

        ASSERT_NE(command, nullptr);
        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::DeleteContentCommentCommand*>(command.get()), nullptr);
    }

    TEST_F(ContentCommentsCommandFactory_Should, ReturnNullptrForDeleteWithInvalidContentCommentId)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "invalid-guid" };

        EXPECT_CALL(*m_mock_guid_parser, isGuid(request.m_routing_path.at(0)))
            .WillOnce(::testing::Return(false));

        auto command = m_sut->createDeleteCommand(request);

        EXPECT_EQ(command, nullptr);
    }

    TEST_F(ContentCommentsCommandFactory_Should, ReturnNullptrForPatchCommand)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "11223344-5566-7788-99aa-bbccddeeff00" };

        auto command = m_sut->createPatchCommand(request);

        EXPECT_EQ(command, nullptr);
    }

    TEST_F(ContentCommentsCommandFactory_Should, ReturnNullptrForPutCommand)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "11223344-5566-7788-99aa-bbccddeeff00" };
        request.m_body = R"({"user_name": "UpdatedUser", "body": "Updated comment"})";

        auto command = m_sut->createPutCommand(request);

        EXPECT_EQ(command, nullptr);
    }

}