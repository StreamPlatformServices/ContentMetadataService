#include "CommandSelector_Should.hpp"

namespace ContentMetadataApi_Tests
{
    TEST_F(CommandSelector_Should, ReturnGetCommandWhenMethodIsGet)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_method = ContentMetadataApi::HttpMethod::Get;
        request.m_root_endpoint = "content";

        EXPECT_CALL(*m_mock_http_command_factories_factory, createHttpCommandFactory("content"))
            .WillOnce([this](const std::string& a_root_endpoint)
                {
                    return std::move(m_mock_http_command_factory);
                });

        EXPECT_CALL(*m_mock_http_command_factory, createGetCommand(::testing::An<const ContentMetadataApi::HttpRequest&>())).Times(1);

        m_sut->selectHttpCommand(request);
    }

    TEST_F(CommandSelector_Should, ReturnPostCommandWhenMethodIsPost)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_method = ContentMetadataApi::HttpMethod::Post;
        request.m_root_endpoint = "content";

        EXPECT_CALL(*m_mock_http_command_factories_factory, createHttpCommandFactory("content"))
            .WillOnce([this](const std::string& a_root_endpoint)
                {
                    return std::move(m_mock_http_command_factory);
                });

        EXPECT_CALL(*m_mock_http_command_factory, createPostCommand(::testing::An<const ContentMetadataApi::HttpRequest&>())).Times(1);

        m_sut->selectHttpCommand(request);
    }

    TEST_F(CommandSelector_Should, ReturnPutCommandWhenMethodIsPut)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_method = ContentMetadataApi::HttpMethod::Put;
        request.m_root_endpoint = "content";

        EXPECT_CALL(*m_mock_http_command_factories_factory, createHttpCommandFactory("content"))
            .WillOnce([this](const std::string& a_root_endpoint)
                {
                    return std::move(m_mock_http_command_factory);
                });

        EXPECT_CALL(*m_mock_http_command_factory, createPutCommand(::testing::An<const ContentMetadataApi::HttpRequest&>())).Times(1);

        m_sut->selectHttpCommand(request);
    }

    TEST_F(CommandSelector_Should, ReturnDeleteCommandWhenMethodIsDelete)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_method = ContentMetadataApi::HttpMethod::Delete;
        request.m_root_endpoint = "content";

        EXPECT_CALL(*m_mock_http_command_factories_factory, createHttpCommandFactory("content"))
            .WillOnce([this](const std::string& a_root_endpoint)
                {
                    return std::move(m_mock_http_command_factory);
                });

        EXPECT_CALL(*m_mock_http_command_factory, createDeleteCommand(::testing::An<const ContentMetadataApi::HttpRequest&>())).Times(1);

        m_sut->selectHttpCommand(request);
    }

    TEST_F(CommandSelector_Should, ReturnNullptrCommandWhenMethodIsPatch)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_method = ContentMetadataApi::HttpMethod::Patch;
        request.m_root_endpoint = "content";

        EXPECT_CALL(*m_mock_http_command_factories_factory, createHttpCommandFactory("content"))
            .WillOnce([this](const std::string& a_root_endpoint)
                {
                    return std::move(m_mock_http_command_factory);
                });

        EXPECT_CALL(*m_mock_http_command_factory, createPatchCommand(::testing::_))
            .WillOnce(::testing::Return(nullptr));

        auto command = m_sut->selectHttpCommand(request);
        EXPECT_EQ(command, nullptr);
    }

    TEST_F(CommandSelector_Should, ReturnNullptrWhenFactoryReturnsNullptr)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_method = ContentMetadataApi::HttpMethod::Get;
        request.m_root_endpoint = "unknown";

        EXPECT_CALL(*m_mock_http_command_factories_factory, createHttpCommandFactory("unknown"))
            .WillOnce(::testing::Return(nullptr));

        auto command = m_sut->selectHttpCommand(request);
        EXPECT_EQ(command, nullptr);
    }

    TEST_F(CommandSelector_Should, ReturnNullptrForUnsupportedHttpMethod)
    {
        ContentMetadataApi::HttpRequest request;
        request.m_method = static_cast<ContentMetadataApi::HttpMethod>(999);
        request.m_root_endpoint = "content";

        EXPECT_CALL(*m_mock_http_command_factories_factory, createHttpCommandFactory("content"))
            .WillOnce(::testing::Return(std::make_unique<Mocks::HttpCommandFactoryMock>()));

        auto command = m_sut->selectHttpCommand(request);
        EXPECT_EQ(command, nullptr);
    }
}