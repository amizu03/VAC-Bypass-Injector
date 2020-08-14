#pragma once
#include "constants.hpp"

namespace global
{
    namespace handle
    {
        extern HANDLE thread;
        extern HANDLE proc;
        extern HWND window;
    } // namespace handle

    namespace alloc
    {
        extern void* dll_path;
    } // namespace alloc

    namespace info {
        extern DWORD pid;
    }

    namespace modules {
        extern std::map< std::string, std::pair< uintptr_t, uintptr_t > > list;

        void enumerate( );
    }
} // namespace global