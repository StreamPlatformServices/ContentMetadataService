#include "ContentsDatabaseService_Should.hpp"

namespace ContentMetadataRepository_Tests
{
    TEST_F(ContentsDatabaseService_Should, GetAllContentsAsync_CommitsTransactionOnSuccess)
    {
        int limit = 10;
        int offset = 0;
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();

        ContentMetadataCore::Entities::Content content_mock;
        content_mock.m_uuid = boost::uuids::random_generator()();
        content_mock.m_title = "Test Title";
        content_mock.m_upload_time = std::chrono::system_clock::now();
        content_mock.m_duration = 120;
        content_mock.m_description = "Test Description";
        content_mock.m_owner_id = boost::uuids::random_generator()();
        content_mock.m_video_file_id = boost::uuids::random_generator()();
        content_mock.m_image_file_id = boost::uuids::random_generator()();

        std::vector<ContentMetadataCore::Entities::Content> content_list_mock = { content_mock };

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, contentCommentRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>
                {
                    co_return std::move(content_comment_repo_mock);
                }));

        EXPECT_CALL(*content_repo_mock, getAllAsync(limit, offset))
            .WillOnce(::testing::Invoke([&](int, int) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>
                {
                    co_return content_list_mock;
                }));

        EXPECT_CALL(*license_rules_repo_mock, getByContentIdAsync(::testing::_))
            .WillRepeatedly(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::LicenseRules>>
                {
                    co_return std::vector<ContentMetadataCore::Entities::LicenseRules>();
                }));

        EXPECT_CALL(*content_comment_repo_mock, getByContentIdAsync(::testing::_))
            .WillRepeatedly(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::ContentComment>>
                {
                    co_return std::vector<ContentMetadataCore::Entities::ContentComment>();
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_)).Times(0);

        std::vector<ContentMetadataCore::Entities::Content> result;

        EXPECT_NO_THROW({
            auto future = boost::asio::co_spawn(
                boost::asio::system_executor(),
                m_sut->getAllContentsAsync(limit, offset),
                boost::asio::use_future
            );

            result = future.get();
            });

        EXPECT_EQ(result.size(), content_list_mock.size());

        EXPECT_EQ(result[0].m_uuid, content_mock.m_uuid);
        EXPECT_EQ(result[0].m_title, content_mock.m_title);
        EXPECT_EQ(result[0].m_duration, content_mock.m_duration);
        EXPECT_EQ(result[0].m_description, content_mock.m_description);
        EXPECT_EQ(result[0].m_owner_id, content_mock.m_owner_id);
        EXPECT_EQ(result[0].m_video_file_id, content_mock.m_video_file_id);
        EXPECT_EQ(result[0].m_image_file_id, content_mock.m_image_file_id);

        EXPECT_NEAR(std::chrono::duration_cast<std::chrono::seconds>(result[0].m_upload_time.time_since_epoch()).count(),
            std::chrono::duration_cast<std::chrono::seconds>(content_mock.m_upload_time.time_since_epoch()).count(),
            1);
    }

    TEST_F(ContentsDatabaseService_Should, GetContentMetadataByIdAsync_CommitsTransactionOnSuccess)
    {
        boost::uuids::uuid content_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        ContentMetadataCore::Entities::Content content_mock;
        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();
        auto content_comment_repo_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, contentCommentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>
                {
                    co_return std::move(content_comment_repo_mock);
                }));

        EXPECT_CALL(*content_repo_mock, getByIdAsync(content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<ContentMetadataCore::Entities::Content>
                {
                    co_return content_mock;
                }));

        EXPECT_CALL(*license_rules_repo_mock, getByContentIdAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::LicenseRules>>
                {
                    co_return std::vector<ContentMetadataCore::Entities::LicenseRules>();
                }));

        EXPECT_CALL(*content_comment_repo_mock, getByContentIdAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::ContentComment>>
                {
                    co_return std::vector<ContentMetadataCore::Entities::ContentComment>();
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_)).Times(0);

        ContentMetadataCore::Entities::Content result;

        EXPECT_NO_THROW({
            auto future = boost::asio::co_spawn(
                boost::asio::system_executor(),
                m_sut->getContentMetadataByIdAsync(content_id),
                boost::asio::use_future
            );

            result = future.get();
            });

        EXPECT_EQ(result.m_uuid, content_mock.m_uuid);
        EXPECT_EQ(result.m_title, content_mock.m_title);
        EXPECT_EQ(result.m_duration, content_mock.m_duration);
        EXPECT_EQ(result.m_description, content_mock.m_description);
        EXPECT_EQ(result.m_owner_id, content_mock.m_owner_id);
        EXPECT_EQ(result.m_video_file_id, content_mock.m_video_file_id);
        EXPECT_EQ(result.m_image_file_id, content_mock.m_image_file_id);

        EXPECT_NEAR(std::chrono::duration_cast<std::chrono::seconds>(result.m_upload_time.time_since_epoch()).count(),
            std::chrono::duration_cast<std::chrono::seconds>(content_mock.m_upload_time.time_since_epoch()).count(),
            1);
    }

    TEST_F(ContentsDatabaseService_Should, GetContentMetadataByOwnerIdAsync_CommitsTransactionOnSuccess)
    {
        boost::uuids::uuid owner_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        std::vector<ContentMetadataCore::Entities::Content> content_list_mock;

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*content_repo_mock, getByOwnerIdAsync(owner_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::Content>>
                {
                    co_return content_list_mock;
                }));

        EXPECT_CALL(*license_rules_repo_mock, getByContentIdAsync(::testing::_))
            .WillRepeatedly(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<std::vector<ContentMetadataCore::Entities::LicenseRules>>
                {
                    co_return std::vector<ContentMetadataCore::Entities::LicenseRules>();
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_)).Times(0);

        std::vector<ContentMetadataCore::Entities::Content> result;

        EXPECT_NO_THROW({
            auto future = boost::asio::co_spawn(
                boost::asio::system_executor(),
                m_sut->getContentMetadataByOwnerIdAsync(owner_id),
                boost::asio::use_future
            );

            result = future.get();
            });

        EXPECT_EQ(result.size(), content_list_mock.size());
    }

    TEST_F(ContentsDatabaseService_Should, EditContentMetadataAsync_CommitsTransactionOnSuccess)
    {
        boost::uuids::uuid content_id = boost::uuids::random_generator()();
        boost::uuids::uuid license_rules_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();

        ContentMetadataCore::Entities::Content content;
        content.m_license_rules = { ContentMetadataCore::Entities::LicenseRules() };

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*license_rules_repo_mock, deleteByContentIdAsync(content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*content_repo_mock, updateAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](ContentMetadataCore::Entities::Content) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*license_rules_repo_mock, addAsync(::testing::_))
            .WillRepeatedly(::testing::Invoke([&](ContentMetadataCore::Entities::LicenseRules) -> boost::asio::awaitable<boost::uuids::uuid>
                {
                    co_return license_rules_id;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_)).Times(0);

        EXPECT_NO_THROW({
            auto future = boost::asio::co_spawn(
                boost::asio::system_executor(),
                m_sut->editContentMetadataAsync(content_id, content),
                boost::asio::use_future
            );

            future.get();
            });
    }

    TEST_F(ContentsDatabaseService_Should, AddContentMetadataAsync_CommitsTransactionOnSuccess)
    {
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        boost::uuids::uuid content_id_mock = boost::uuids::random_generator()();
        boost::uuids::uuid license_rules_id_mock = boost::uuids::random_generator()();

        ContentMetadataCore::Entities::Content content;
        content.m_license_rules = { ContentMetadataCore::Entities::LicenseRules() };

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::An<const boost::uuids::uuid&>()))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*content_repo_mock, addAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const ContentMetadataCore::Entities::Content&) -> boost::asio::awaitable<boost::uuids::uuid>
                {
                    co_return content_id_mock;
                }));

        EXPECT_CALL(*license_rules_repo_mock, addAsync(::testing::_))
            .WillRepeatedly(::testing::Invoke([&](ContentMetadataCore::Entities::LicenseRules) -> boost::asio::awaitable<boost::uuids::uuid>
                {
                    co_return license_rules_id_mock;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_)).Times(0);

        boost::uuids::uuid result_uuid;

        EXPECT_NO_THROW({
            auto future = boost::asio::co_spawn(
                boost::asio::system_executor(),
                m_sut->addContentMetadataAsync(content),
                boost::asio::use_future
            );

            result_uuid = future.get();
            });

        EXPECT_EQ(result_uuid, content_id_mock);
    }

    TEST_F(ContentsDatabaseService_Should, DeleteContentMetadataAsync_CommitsTransactionOnSuccess)
    {
        boost::uuids::uuid content_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*content_repo_mock, deleteAsync(content_id))
            .WillOnce(::testing::Invoke([&](boost::uuids::uuid) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<void>
                {
                    co_return;
                }));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_)).Times(0);

        EXPECT_NO_THROW({
            auto future = boost::asio::co_spawn(
                boost::asio::system_executor(),
                m_sut->deleteContentMetadataAsync(content_id),
                boost::asio::use_future
            );

            future.get();
            });
    }


    TEST_F(ContentsDatabaseService_Should, GetAllContentsAsync_RollsBackTransactionOnException)
    {
        int limit = 10;
        int offset = 0;
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();
        auto content_comments_mock = std::make_unique<Mocks::ContentCommentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, contentCommentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentCommentRepository>>
                {
                    co_return std::move(content_comments_mock);
                }));

        EXPECT_CALL(*content_repo_mock, getAllAsync(limit, offset))
            .WillOnce(::testing::Throw(std::runtime_error("Error fetching content")));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_));
        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_)).Times(0);

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->getAllContentsAsync(limit, offset),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }

    TEST_F(ContentsDatabaseService_Should, GetContentMetadataByOwnerIdAsync_RollsBackTransactionOnException)
    {
        boost::uuids::uuid owner_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*content_repo_mock, getByOwnerIdAsync(owner_id))
            .WillOnce(::testing::Throw(std::runtime_error("Error fetching content")));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_));
        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_)).Times(0);

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->getContentMetadataByOwnerIdAsync(owner_id),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }


    TEST_F(ContentsDatabaseService_Should, EditContentMetadataAsync_RollsBackTransactionOnException)
    {
        boost::uuids::uuid content_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();

        ContentMetadataCore::Entities::Content content;

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));

        EXPECT_CALL(*license_rules_repo_mock, deleteByContentIdAsync(content_id))
            .WillOnce(::testing::Throw(std::runtime_error("Error deleting license rules")));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_));
        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_)).Times(0);

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->editContentMetadataAsync(content_id, content),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }

    TEST_F(ContentsDatabaseService_Should, AddContentMetadataAsync_RollsBackTransactionOnException)
    {
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();
        ContentMetadataCore::Entities::Content content;

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();
        auto license_rules_repo_mock = std::make_unique<Mocks::LicenseRulesRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*m_unit_of_work_mock, licenseRulesRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::ILicenseRulesRepository>>
                {
                    co_return std::move(license_rules_repo_mock);
                }));


        EXPECT_CALL(*content_repo_mock, addAsync(::testing::_))
            .WillOnce(::testing::Throw(std::runtime_error("Error adding content")));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_));
        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_)).Times(0);

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->addContentMetadataAsync(content),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }

    TEST_F(ContentsDatabaseService_Should, DeleteContentMetadataAsync_RollsBackTransactionOnException)
    {
        boost::uuids::uuid content_id = boost::uuids::random_generator()();
        boost::uuids::uuid transaction_id_mock = boost::uuids::random_generator()();

        auto content_repo_mock = std::make_unique<Mocks::ContentRepositoryMock>();

        EXPECT_CALL(*m_unit_of_work_mock, contentRepositoryAsync(::testing::_))
            .WillOnce(::testing::Invoke([&](const boost::uuids::uuid&) -> boost::asio::awaitable<std::unique_ptr<ContentMetadataRepository::IContentRepository>>
                {
                    co_return std::move(content_repo_mock);
                }));

        EXPECT_CALL(*content_repo_mock, deleteAsync(content_id))
            .WillOnce(::testing::Throw(std::runtime_error("Error deleting content")));

        EXPECT_CALL(*m_unit_of_work_mock, rollbackTransaction(::testing::_));
        EXPECT_CALL(*m_unit_of_work_mock, commitTransactionAsync(::testing::_)).Times(0);

        auto future = boost::asio::co_spawn(
            boost::asio::system_executor(),
            m_sut->deleteContentMetadataAsync(content_id),
            boost::asio::use_future
        );

        EXPECT_THROW(future.get(), std::runtime_error);
    }
} // namespace ContentMetadataRepository_Tests
