#include "vars.hpp"

/* ghetto fix to compile without wide char versions of these structs & functions */
#undef MODULEENTRY32
#undef Module32First
#undef Module32Next

namespace global
{
    namespace handle
    {
        HANDLE thread = INVALID_HANDLE_VALUE;
        HANDLE proc = INVALID_HANDLE_VALUE;
        HWND window = nullptr;
    } // namespace handle

    namespace alloc
    {
        void* dll_path = nullptr;
    } // namespace alloc

    namespace info {
        DWORD pid = 0;
    }

    namespace modules {
        std::map< std::string, std::pair< uintptr_t, uintptr_t > > list { };

        void enumerate( ) {
            auto snap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, info::pid );

            if ( snap != INVALID_HANDLE_VALUE ) {
                MODULEENTRY32 me;
                me.dwSize = sizeof( me );

                if ( Module32First( snap, &me ) ) {
                    do {
                        list [ me.szModule ] = std::pair< uintptr_t, uintptr_t >( reinterpret_cast< uintptr_t >( me.modBaseAddr ), me.modBaseSize );
                    } while ( Module32Next( snap, &me ) );
                }
            }

            CloseHandle( snap );
        }
    }
} // namespace global