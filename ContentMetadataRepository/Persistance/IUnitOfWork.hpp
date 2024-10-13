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

        virtual boost::asio::awaitable<std::unique_ptr<IContentRepository>> contentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) = 0;
        virtual boost::asio::awaitable<std::unique_ptr<IContentCommentRepository>> contentCommentRepositoryAsync(const boost::uuids::uuid& a_transaction_id) = 0;
        virtual boost::asio::awaitable<std::unique_ptr<ILicenseRulesRepository>> licenseRulesRepositoryAsync(const boost::uuids::uuid& a_transaction_id) = 0;
    };
}