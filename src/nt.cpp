#include "nt.hpp"
#include "global.hpp"

namespace global
{
    namespace nt
    {
        DWORD unhook(const std::string_view &func_name, std::vector<uint8_t> &o_bytes)
        {
            DWORD ret_protection = 0;

            const auto api_func = GetProcAddress(LoadLibraryA("ntdll.dll"), func_name.data());

            if (!api_func)
                return error("Failed to locate target NT API function.");

            std::array<uint8_t, 5> o_instructions;
            std::array<uint8_t, 5> o_instructions_game;

            std::fill(o_instructions.begin(), o_instructions.end(), 0x90);
            std::fill(o_instructions_game.begin(), o_instructions_game.end(), 0x90);

            memcpy(o_instructions.data(), api_func, o_instructions.size());

            SIZE_T bytes_read = 0;
            auto status = ReadProcessMemory(handle::proc, api_func, o_instructions_game.data(), o_instructions_game.size(), &bytes_read);

            if (!bytes_read || !status)
                return error("Failed to unhook NT API function.");

            /* backup original  hook instructions */
            for (const auto &hook_byte : o_instructions_game)
                o_bytes.push_back(hook_byte);

            /* temporarily force game to use our version of the function they hook */
            DWORD o_protection = 0;
            VirtualProtectEx(handle::proc, api_func, o_instructions.size(), PAGE_EXECUTE_READWRITE, &ret_protection);

            SIZE_T bytes_written = 0;
            status = WriteProcessMemory(handle::proc, api_func, o_instructions.data(), o_instructions.size(), &bytes_written);

            if (!bytes_written || !status)
                return error("Failed to unhook NT API function.");

            return ret_protection;
        }

        void rehook(const std::string_view &func_name, std::vector<uint8_t> &o_bytes, DWORD old_protection)
        {
            const auto api_func = GetProcAddress(LoadLibraryA("ntdll.dll"), func_name.data());

            if (!api_func)
                error("Failed to locate target NT API function.");

            VirtualProtectEx(handle::proc, api_func, o_bytes.size(), old_protection, &old_protection);

            SIZE_T bytes_written = 0;
            const auto status = WriteProcessMemory(handle::proc, api_func, o_bytes.data(), o_bytes.size(), &bytes_written);

            if (!bytes_written || !status)
                error("Failed to rehook NT API function.");
        }
    } // namespace nt
} // namespace global