#include "vars.hpp"

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
        void *dll_path = nullptr;
    } // namespace alloc
} // namespace global