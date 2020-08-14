#pragma once
#include "vars.hpp"

namespace global
{
    namespace nt
    {
        DWORD unhook( const std::string_view& func_name, std::vector<uint8_t>& o_bytes );
        void rehook( const std::string_view& func_name, std::vector<uint8_t>& o_bytes, DWORD old_protection );
    } // namespace nt
} // namespace global