#include "util.hpp"

namespace global {
    namespace util {
        pattern::pattern( const std::string_view& name, const std::string_view& pat ) {
            auto pat1 = const_cast< char* >( pat.data( ) );

            const auto mod_addr = modules::list [ name.data( ) ].first;
            const auto mod_len = modules::list [ name.data( ) ].second;
            const auto game_dat = new std::uint8_t [ mod_len ];

            memset( game_dat, 0, mod_len );

            ReadProcessMemory( handle::proc, reinterpret_cast< void* >( mod_addr ), game_dat, mod_len, nullptr );

            std::uintptr_t first_match = 0;

            for ( std::uintptr_t current_address = 0; current_address < mod_len; current_address++ ) {
                if ( !*pat1 ) {
                    delete [ ] game_dat;
                    this->m_addr = mod_addr + first_match;
                    return;
                }

                if ( *reinterpret_cast< std::uint8_t* >( pat1 ) == '\?' || game_dat [ current_address ] == get_byte( reinterpret_cast< uint8_t* > ( pat1 ) ) ) {
                    if ( !first_match )
                        first_match = current_address;

                    if ( !pat1 [ 2 ] ) {
                        delete [ ] game_dat;
                        this->m_addr = mod_addr + first_match;
                        return;
                    }

                    if ( *reinterpret_cast< std::uint16_t* >( pat1 ) == '\?\?' || *reinterpret_cast< std::uint8_t* >( pat1 ) != '\?' )
                        pat1 += 3;
                    else
                        pat1 += 2;
                }
                else {
                    pat1 = const_cast< char* >( pat.data( ) );
                    first_match = 0;
                }
            }

            delete [ ] game_dat;

            this->m_addr = 0;
            return;
        }
    }
}