#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks/ContentCommentRepositoryMock.hpp"
#include "Mocks/UnitOfWorkMock.hpp"
#include "../../ContentMetadataRepository/ContentCommentsDatabaseService.hpp"

namespace ContentMetadataRepository_Tests
{
    class ContentCommentsDatabaseService_Should : public ::testing::Test 
    {
    protected:
        std::shared_ptr< ::testing::NiceMock<Mocks::UnitOfWorkMock>> m_unit_of_work_mock;
        std::unique_ptr<ContentMetadataRepository::ContentCommentsDatabaseService> m_sut;

        void SetUp() override 
        {
            m_unit_of_work_mock = std::make_shared<::testing::NiceMock<Mocks::UnitOfWorkMock>>();
            m_sut = std::make_unique<ContentMetadataRepository::ContentCommentsDatabaseService>(m_unit_of_work_mock);
        }
    };
}