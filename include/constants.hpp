#pragma once
#include "includes.hpp"

namespace global
{
    constexpr auto PACCESS_RIGHTS = PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_CREATE_THREAD;
    constexpr auto DLL_NAME = "test.dll";
} // namespace global