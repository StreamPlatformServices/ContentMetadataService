#pragma once
#include "Repositories/ContentComments/IContentCommentRepository.hpp"
#include "Repositories/Contents/IContentRepository.hpp"
#include "Repositories/LicenseRules/ILicenseRulesRepository.hpp"

namespace ContentMetadataRepository
{
    class IUnitOfWork 
    {
    public:
        virtual ~IUnitOfWork() = default;

        virtual boost::asio::awaitable<void> commitTransactionAsync(const boost::uuids::uuid& a_transaction_id) = 0;
        virtual void rollbackTransaction(const boost::uuids::uuid& a_transaction_id) = 0;

        virtual auto contentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<IContentRepository>> = 0;
        virtual auto contentCommentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<IContentCommentRepository>> = 0;
        virtual auto licenseRulesRepositoryAsync(const boost::uuids::uuid& a_transaction_id) -> boost::asio::awaitable<std::unique_ptr<ILicenseRulesRepository>> = 0;
    };
}