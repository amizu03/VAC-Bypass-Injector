#pragma once
#include "global.hpp"

namespace global {
    namespace util {
        constexpr bool in_range( const uint8_t x, const uint8_t a, const uint8_t b ) {
            return x >= a && x <= b;
        }

        constexpr uint8_t get_bits( const uint8_t x ) {
            return in_range( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( in_range( x, '0', '9' ) ? x - '0' : 0 );
        }

        constexpr uint8_t get_byte( const uint8_t* x ) {
            return get_bits( x [ 0 ] ) << 4 | get_bits( x [ 1 ] );
        }

        class pattern {
            uintptr_t m_addr;

        public:
            pattern( uintptr_t addr ) {
                this->m_addr = addr;
            }

            pattern( const std::string_view& name, const std::string_view& pat );

            template <typename type>
            type get( ) {
                return reinterpret_cast< type >( this->m_addr );
            }

            pattern sub( uintptr_t offset ) {
                return pattern( this->m_addr - offset );
            }

            pattern add( uintptr_t offset ) {
                return pattern( this->m_addr + offset );
            }

            pattern deref( ) {
                uintptr_t val;
                ReadProcessMemory( handle::proc, reinterpret_cast< void* > ( this->m_addr ), &val, sizeof( val ), nullptr );
                return pattern( val );
            }

            pattern resolve_rip( ) {
                uint32_t val;
                ReadProcessMemory( handle::proc, reinterpret_cast< void* > ( this->m_addr + 1 ), &val, sizeof( val ), nullptr );
                return pattern( this->m_addr + val + 5 );
            }
        };
    }
}