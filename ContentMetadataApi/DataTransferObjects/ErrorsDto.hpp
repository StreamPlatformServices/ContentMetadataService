#pragma once
#include "../StdAfx.hpp"
#include "../Visitors/IVisitable.hpp"

namespace ContentMetadataApi
{
    namespace Dto
    {
        struct ErrorsDto : public Visitors::IVisitable
        {
            std::vector<std::string> m_errors;

            void accept(Visitors::IVisitor& a_visitor) override
            {
                a_visitor.visit(*this);
            }
        };
    }
}