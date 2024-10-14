#pragma once
#include "../StdAfx.hpp"
#include "../Visitors/IVisitable.hpp"
#include "../../ContentMetadataCore/Enums/LicenseDuration.hpp"
#include "../../ContentMetadataCore/Enums/LicenseType.hpp"

namespace ContentMetadataApi
{
    namespace Dto
    {
        struct LicenseRulesDto : public Visitors::IVisitable
        {
            boost::uuids::uuid                                          m_uuid = boost::uuids::nil_uuid();
            int                                                         m_price = 0;
            ContentMetadataCore::Enums::LicenseType                     m_type = ContentMetadataCore::Enums::LicenseType::Unknown;
            std::optional<ContentMetadataCore::Enums::LicenseDuration>  m_duration;
            boost::uuids::uuid                                          m_content_id = boost::uuids::nil_uuid();

            void accept(Visitors::IVisitor& a_visitor) override
            {
                a_visitor.visit(*this);
            }
        };
    }
}