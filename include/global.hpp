#pragma once
#include "nt.hpp"

namespace global
{
    int error( const std::string_view& err );
    void free_resources( );
} // namespace global