#pragma region Includes & Macros

// C++ Includes
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <sstream>
#include <map>

// Minhook
#include "Libs/minhook/minhook.h"

// Utils
#include "SDK/Globals.h"
#include "SDK/Utils.h"

// Hooks
#include "SDK/Hook/FuncHook.h"

#pragma endregion

// This function initializes the client.
void InitializeClient(HMODULE module) {
    InitializeHooks();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH) {
        // Prevent the DLL from being unloaded
        DisableThreadLibraryCalls(module);

        // Create a new thread to initialize the client on
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitializeClient, module, 0, nullptr);
    }
    return true;
}