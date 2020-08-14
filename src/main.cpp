#include "global.hpp"

int main()
{
    using namespace global;

    namespace fs = std::filesystem;

    std::string path;

    const auto cur_path = fs::current_path();

    for (const auto &entry : fs::directory_iterator(cur_path))
    {
        /* find the first DLL that appears, and let's use that one */
        if (entry.is_regular_file() && entry.path().extension() == ".dll")
        {
            path = entry.path().string();
            break;
        }
    }

    if (path.empty())
        return error("Failed to locate DLL in current directory.");

    /* the actual injector itself */
    handle::window = FindWindowA(nullptr, "Counter-Strike: Global Offensive");

    if (!handle::window)
        return error("Failed to find remote process window.");

    DWORD pid = 0;
    GetWindowThreadProcessId(handle::window, &pid);

    if (!pid)
        return error("Failed to retrieve remote process ID.");

    handle::proc = OpenProcess(PACCESS_RIGHTS, false, pid);

    if (handle::proc == INVALID_HANDLE_VALUE)
        return error("Failed to open handle to remote process.");

    alloc::dll_path = VirtualAllocEx(handle::proc, nullptr, path.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (!alloc::dll_path)
        return error("Failed to allocate memory in remote process.");

    SIZE_T bytes_written = 0;
    auto status = WriteProcessMemory(handle::proc, alloc::dll_path, path.data(), path.size(), &bytes_written);

    if (!bytes_written || !status)
        return error("Failed to write DLL path to remote process memory.");

    /* bypass NtOpenFile and NtQueryInformationThread hook to allow LoadLibrary and manualmap injection and prevent thread from being queried */
    std::vector<uint8_t> o_NtOpenFile{};
    std::vector<uint8_t> o_NtQueryInformationThread{};

    const auto o_prot_NtOpenFile = nt::unhook("NtOpenFile", o_NtOpenFile);
    const auto o_prot_NtQueryInformationThread = nt::unhook("NtQueryInformationThread", o_NtQueryInformationThread);

    /* create thread to initialize injected module */
    handle::thread = CreateRemoteThread(handle::proc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), alloc::dll_path, 0, nullptr);

    if (handle::thread == INVALID_HANDLE_VALUE)
        return error("Failed to create thread in remote process.");

    if (WaitForSingleObject(handle::thread, INFINITE) == WAIT_FAILED)
        return error("Failed to wait for remote thread termination.");

    /* restore anticheat hooks so it doesn't get too suspicious */
    nt::rehook("NtOpenFile", o_NtOpenFile, o_prot_NtOpenFile);
    nt::rehook("NtQueryInformationThread", o_NtQueryInformationThread, o_prot_NtQueryInformationThread);

    free_resources();

    std::cout << "Success." << std::endl;
    std::cin.get();

    return 0;
}