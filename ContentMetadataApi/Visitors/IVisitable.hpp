#pragma once
#include "IVisitor.hpp"

namespace ContentMetadataApi
{
    namespace Visitors
    {
        class IVisitable
        {
        public:
            virtual ~IVisitable() = default;

            virtual void accept(IVisitor& a_visitor) = 0;
        };
    }
}