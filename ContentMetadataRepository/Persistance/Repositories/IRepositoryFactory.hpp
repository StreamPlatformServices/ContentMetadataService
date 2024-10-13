#pragma once
#include "ContentComments/IContentCommentRepository.hpp"
#include "Contents/IContentRepository.hpp"
#include "LicenseRules/ILicenseRulesRepository.hpp"

namespace ContentMetadataRepository
{
    class IRepositoryFactory {
    public:
        virtual ~IRepositoryFactory() = default;

        virtual auto createContentCommentRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<IContentCommentRepository> = 0;
        virtual auto createContentRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<IContentRepository> = 0;
        virtual auto createLicenseRulesRepository(std::shared_ptr<pqxx::work> a_transaction) const -> std::unique_ptr<ILicenseRulesRepository> = 0;
    };

}
