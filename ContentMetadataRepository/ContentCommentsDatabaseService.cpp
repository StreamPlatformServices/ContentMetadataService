#include "ContentCommentsDatabaseService.hpp"

namespace ContentMetadataRepository
{
    ContentCommentsDatabaseService::ContentCommentsDatabaseService(std::shared_ptr<IUnitOfWork> a_unit_of_work) noexcept
        : m_unit_of_work(a_unit_of_work)
    {
    }


    auto ContentCommentsDatabaseService::addContentCommentAsync(ContentMetadataCore::Entities::ContentComment& a_content_comment) -> boost::asio::awaitable<boost::uuids::uuid>
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto contentCommentRepository = co_await m_unit_of_work->contentCommentRepositoryAsync(transaction_id);

        try
        {
            auto content_id = co_await contentCommentRepository->addAsync(a_content_comment);
            co_await m_unit_of_work->commitTransactionAsync(transaction_id);
            co_return content_id;
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }

    auto ContentCommentsDatabaseService::deleteContentCommentAsync(const boost::uuids::uuid& a_content_comment_id) -> boost::asio::awaitable<void>
    {
        auto transaction_id = boost::uuids::random_generator()();
        auto contentCommentRepository = co_await m_unit_of_work->contentCommentRepositoryAsync(transaction_id);
        
        try
        {
            co_await contentCommentRepository->deleteAsync(a_content_comment_id);
            co_await m_unit_of_work->commitTransactionAsync(transaction_id);
        }
        catch (...)
        {
            m_unit_of_work->rollbackTransaction(transaction_id);
            throw;
        }
    }
}
