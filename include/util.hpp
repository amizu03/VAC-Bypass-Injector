#pragma once
#include "global.hpp"

namespace global {
    namespace util {
        template <typename type>
        constexpr auto in_range( const type& x, const type& a, const type& b ) {
            return x >= a && x <= b;
        }

        template <typename type>
        constexpr auto get_bits( const type& x ) {
            return in_range( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( in_range( x, '0', '9' ) ? x - '0' : 0 );
        }

        template <typename type>
        constexpr auto get_byte( const type& x ) {
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