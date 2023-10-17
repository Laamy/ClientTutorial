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
#include "SDK/Console.h"
#include "SDK/Globals.h"
#include "SDK/Utils.h"

// Maths
#include "SDK/Maths/TextHolder.h"
#include "SDK/Maths/UIColor.h"
#include "SDK/Maths/Vector2.h"
#include "SDK/Maths/Vector3.h"
#include "SDK/Maths/Vector4.h"

// Hooks
#include "SDK/Hook/FuncHook.h"

#pragma endregion

// This function initializes the client.
void InitializeClient(HMODULE module) {
    // init console
    Console::CreateConsole("Episode 4: Debugging Utils (Console)");

    // init everything else
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