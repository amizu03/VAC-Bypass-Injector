#include "util.hpp"

int main( ) {
    using namespace global;

    namespace fs = std::filesystem;

    std::string path;

    const auto cur_path = fs::current_path( );

    for ( const auto& entry : fs::directory_iterator( cur_path ) ) {
        /* find the first DLL that appears, and let's use that one */
        if ( entry.is_regular_file( ) && entry.path( ).extension( ) == ".dll" ) {
            path = entry.path( ).string( );
            break;
        }
    }

    if ( path.empty( ) )
        return error( "Failed to locate DLL in current directory." );

    /* the actual injector itself */
    handle::window = FindWindowA( nullptr, "Counter-Strike: Global Offensive" );

    if ( !handle::window )
        return error( "Failed to find remote process window." );

    GetWindowThreadProcessId( handle::window, &info::pid );

    if ( !info::pid )
        return error( "Failed to retrieve remote process ID." );

    handle::proc = OpenProcess( PACCESS_RIGHTS, false, info::pid );

    if ( handle::proc == INVALID_HANDLE_VALUE )
        return error( "Failed to open handle to remote process." );

    modules::enumerate( );

    alloc::dll_path = VirtualAllocEx( handle::proc, nullptr, path.size( ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );

    if ( !alloc::dll_path )
        return error( "Failed to allocate memory in remote process." );

    SIZE_T bytes_written = 0;
    auto status = WriteProcessMemory( handle::proc, alloc::dll_path, path.data( ), path.size( ), &bytes_written );

    if ( !bytes_written || !status )
        return error( "Failed to write DLL path to remote process memory." );

    /* bypass NtOpenFile hook to allow LoadLibrary calls to pass through and not be queried */
    std::vector<uint8_t> o_NtOpenFile {};
    const auto o_prot_NtOpenFile = nt::unhook( "NtOpenFile", o_NtOpenFile );

    /* bypass thread creation detection */
    auto thread_detection = util::pattern( "client.dll", "FF 15 ? ? ? ? ? ? ? ? ? ? 6A 00 6A 04" ).add( 10 );
    uint8_t backup_byte;

    SIZE_T bytes_read = 0;
    status = ReadProcessMemory( handle::proc, thread_detection.get<void*>( ), &backup_byte, sizeof( backup_byte ), &bytes_read );

    if ( !bytes_read || !status )
        return error( "Failed backup anti-thread creation instructions." );

    if ( backup_byte != 0x74 ) {
        TerminateProcess( handle::proc, 9 );
        return error( "Injector is outdated (DO NOT USE); anti-thread creation has changed since last update.\nTerminating injector and target process." );
    }

    uint8_t replacement_byte = 0xEB;
    bytes_written = 0;
    status = WriteProcessMemory( handle::proc, thread_detection.get<void*>( ), &replacement_byte, sizeof( replacement_byte ), &bytes_written );

    if ( !bytes_written || !status )
        return error( "Failed to patch anti-thread creation." );

    /* create thread to initialize injected module */
    handle::thread = CreateRemoteThread( handle::proc, nullptr, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( LoadLibraryA ), alloc::dll_path, 0, nullptr );

    if ( handle::thread == INVALID_HANDLE_VALUE )
        return error( "Failed to create thread in remote process." );

    if ( WaitForSingleObject( handle::thread, INFINITE ) == WAIT_FAILED )
        return error( "Failed to wait for remote thread termination." );

    /* restore anticheat hooks so it doesn't get too suspicious */
    nt::rehook( "NtOpenFile", o_NtOpenFile, o_prot_NtOpenFile );

    /* restore thread detection so it doesn't look too suspicious */
    bytes_written = 0;
    status = WriteProcessMemory( handle::proc, thread_detection.get<void*>( ), &backup_byte, sizeof( backup_byte ), &bytes_written );

    if ( !bytes_written || !status )
        return error( "Failed to restore anti-thread creation." );

    free_resources( );

    std::cout << "Success." << std::endl;
    std::cin.get( );

    return 0;
}