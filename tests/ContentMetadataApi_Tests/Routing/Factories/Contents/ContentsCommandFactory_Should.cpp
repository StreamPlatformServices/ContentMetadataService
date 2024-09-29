#include "ContentsCommandFactory_Should.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/AddContentCommand.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/GetContentByIdCommand.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/GetContentsCommand.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/GetContentsByOwnerIdCommand.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/DeleteContentCommand.hpp"
#include "../../../../../ContentMetadataApi/Routing/Commands/Contents/UpdateContentCommand.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(ContentsCommandFactory_Should, ReturnGetContentsCommandWhenRoutingPathIsEmpty)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = {};

        auto command = m_sut->createGetCommand(request);

        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::GetContentsCommand*>(command.get()), nullptr);
    }

    TEST_F(ContentsCommandFactory_Should, ReturnGetContentByIdCommandWhenPathContainsValidGuid)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "123e4567-e89b-12d3-a456-426614174000" };

        EXPECT_CALL(*m_mock_guid_parser, isGuid(::testing::An<const std::string&>())).WillOnce(::testing::Return(true));
        EXPECT_CALL(*m_mock_guid_parser, parseGuid(::testing::_)).WillOnce(::testing::Return(boost::uuids::string_generator()("123e4567-e89b-12d3-a456-426614174000")));

        auto command = m_sut->createGetCommand(request);

        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::GetContentByIdCommand*>(command.get()), nullptr);
    }

    TEST_F(ContentsCommandFactory_Should, ReturnGetContentsByOwnerIdCommandWhenPathIsAuthorWithValidGuid)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "author", "123e4567-e89b-12d3-a456-426614174000" };

        EXPECT_CALL(*m_mock_guid_parser, isGuid(::testing::An<const std::string&>())).WillOnce(::testing::Return(true));
        EXPECT_CALL(*m_mock_guid_parser, parseGuid(::testing::_)).WillOnce(::testing::Return(boost::uuids::string_generator()("123e4567-e89b-12d3-a456-426614174000")));

        auto command = m_sut->createGetCommand(request);

        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::GetContentsByOwnerIdCommand*>(command.get()), nullptr);
    }


    TEST_F(ContentsCommandFactory_Should, ReturnNullptrWhenPathContainsInvalidGuid)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "invalid-guid" };

        EXPECT_CALL(*m_mock_guid_parser, isGuid(::testing::An<const std::string&>())).WillOnce(::testing::Return(false));

        auto command = m_sut->createGetCommand(request);

        EXPECT_EQ(command, nullptr);
    }

    TEST_F(ContentsCommandFactory_Should, ReturnAddContentCommandWhenPostRequestIsMadeToRoot)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = {};
        request.m_body = "Test body";

        auto command = m_sut->createPostCommand(request);

        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::AddContentCommand*>(command.get()), nullptr);
    }

    TEST_F(ContentsCommandFactory_Should, ReturnNullptrForPostRequestWithNonEmptyRoutingPath)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "invalidPath" };
        request.m_body = "Test body";

        auto command = m_sut->createPostCommand(request);

        EXPECT_EQ(command, nullptr);
    }


    TEST_F(ContentsCommandFactory_Should, ReturnDeleteContentCommandWhenValidGuidProvided)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "123e4567-e89b-12d3-a456-426614174000" };

        EXPECT_CALL(*m_mock_guid_parser, isGuid(::testing::An<const std::string&>())).WillOnce(::testing::Return(true));
        EXPECT_CALL(*m_mock_guid_parser, parseGuid(::testing::_)).WillOnce(::testing::Return(boost::uuids::string_generator()("123e4567-e89b-12d3-a456-426614174000")));

        auto command = m_sut->createDeleteCommand(request);

        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::DeleteContentCommand*>(command.get()), nullptr);
    }

    TEST_F(ContentsCommandFactory_Should, ReturnNullptrForDeleteRequestWithInvalidGuid)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "invalid-guid" };

        EXPECT_CALL(*m_mock_guid_parser, isGuid(::testing::An<const std::string&>())).WillOnce(::testing::Return(false));

        auto command = m_sut->createDeleteCommand(request);

        EXPECT_EQ(command, nullptr);
    }


    TEST_F(ContentsCommandFactory_Should, ReturnUpdateContentCommandWhenPutRequestContainsValidGuid)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "123e4567-e89b-12d3-a456-426614174000" };
        request.m_body = "Test body";

        EXPECT_CALL(*m_mock_guid_parser, isGuid(::testing::An<const std::string&>())).WillOnce(::testing::Return(true));
        EXPECT_CALL(*m_mock_guid_parser, parseGuid(::testing::_)).WillOnce(::testing::Return(boost::uuids::string_generator()("123e4567-e89b-12d3-a456-426614174000")));

        auto command = m_sut->createPutCommand(request);

        EXPECT_NE(dynamic_cast<ContentMetadataApi::Routing::UpdateContentCommand*>(command.get()), nullptr);
    }

    TEST_F(ContentsCommandFactory_Should, ReturnNullptrForPutRequestWithInvalidGuid)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "invalid-guid" };
        request.m_body = "Test body";

        EXPECT_CALL(*m_mock_guid_parser, isGuid(::testing::An<const std::string&>())).WillOnce(::testing::Return(false));

        auto command = m_sut->createPutCommand(request);

        EXPECT_EQ(command, nullptr);
    }


    TEST_F(ContentsCommandFactory_Should, ReturnNullptrForPatchCommand)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_routing_path = { "patch" };

        auto command = m_sut->createPatchCommand(request);

        EXPECT_EQ(command, nullptr);
    }
}