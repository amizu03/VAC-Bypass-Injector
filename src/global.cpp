#include "global.hpp"

namespace global
{
    int error(const std::string_view &err)
    {
        /* show error to user */
        std::cout << err << std::endl;
        std::cin.get();

        /* free all allocated resources and open handles */
        free_resources();

        /* close process */
        exit(-1);

        /* return error code of -1 */
        return -1;
    }

    void free_resources()
    {
        /* free allocated data*/
        if (alloc::dll_path)
            VirtualFreeEx(handle::proc, alloc::dll_path, 0, MEM_RELEASE);

        /* close all open handles */
        if (handle::thread != INVALID_HANDLE_VALUE)
            CloseHandle(handle::thread);
        if (handle::proc != INVALID_HANDLE_VALUE)
            CloseHandle(handle::proc);
        if (handle::window != INVALID_HANDLE_VALUE)
            CloseHandle(handle::window);
    }
} // namespace global