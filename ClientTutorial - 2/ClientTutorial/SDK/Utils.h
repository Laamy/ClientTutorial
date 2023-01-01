#pragma once

namespace Utils {
#define log(str) OutputDebugString(str)

    template <typename R, typename... Args>
    R CallFunc(void* func, Args... args)
    {
        // Call the function and return the result.
        return ((R(*)(Args...))func)(args...);
    }

    template <typename T>
    std::string combine(T t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    template <typename T, typename... Args>
    std::string combine(T t, Args... args)
    {
        std::stringstream ss;
        ss << t << combine(args...);
        return ss.str();
    }

    template <typename T>
    bool HookFunction(void* pTarget, T pDetour, void* pOriginal) {
        static bool initialized = false;
        if (!initialized)
        {
            initialized = true;
            if (MH_Initialize() != MH_OK)
            {
                log("Error: Failed to initialize MinHook\n");
                return false;
            }
        }

        if (MH_CreateHook(pTarget, pDetour, (LPVOID*)pOriginal) != MH_OK)
        {
            log("Error: Failed to create hook\n");
            return false;
        }

        if (MH_EnableHook(pTarget) != MH_OK)
        {
            log("Error: Failed to enable hook\n");
            return false;
        }

        log(combine("Success: Enabled hook for ", pTarget, "\n").c_str());

        return true;
    }

    #define INRANGE(x, a, b) (x >= a && x <= b)
    #define GET_BYTE(x) ((GET_BITS(x[0]) << 4) | GET_BITS(x[1]))
    #define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? (x - '0') : 0))

    static void* findSig(const char* sig) {
        const char* pattern = sig;
        uintptr_t first_match = 0;
        static const uintptr_t range_start = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));
        static MODULEINFO mi_mod_info;
        static bool init = false;
        if (!init) {
            init = true;
            GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(range_start), &mi_mod_info, sizeof(MODULEINFO));
        }
        static const uintptr_t range_end = range_start + mi_mod_info.SizeOfImage;

        BYTE pat_byte = GET_BYTE(pattern);
        const char* old_pat = pattern;

        for (uintptr_t p_cur = range_start; p_cur < range_end; p_cur++) {
            if (!*pattern) {
                return reinterpret_cast<void*>(first_match);
            }

            while (*(PBYTE)pattern == ' ') {
                pattern++;
            }

            if (!*pattern) {
                return reinterpret_cast<void*>(first_match);
            }

            if (old_pat != pattern) {
                old_pat = pattern;
                if (*(PBYTE)pattern != '\?') {
                    pat_byte = GET_BYTE(pattern);
                }
            }

            if (*(PBYTE)pattern == '\?' || *(BYTE*)p_cur == pat_byte) {
                if (!first_match) {
                    first_match = p_cur;
                }

                if (!pattern[2]) {
                    return reinterpret_cast<void*>(first_match);
                }

                pattern += 2;
            }
            else {
                pattern = sig;
                first_match = 0;
            }
        }
    }
}