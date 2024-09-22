#pragma once
#include "../StdAfx.hpp"
#include "../Visitors/IVisitable.hpp"

namespace ContentMetadataApi
{
    namespace Dto
    {
        struct GuidDto : public Visitors::IVisitable
        {
            boost::uuids::uuid m_uuid = boost::uuids::nil_uuid();

            void accept(Visitors::IVisitor& a_visitor) override
            {
                a_visitor.visit(*this);
            }
        };
    }
}