#pragma once

namespace Utils {
#define log(str) OutputDebugString(str)

    template <typename R, typename... Args>
    R CallFunc(void* func, Args... args)
    {
        // Call the function and return the result.
        return ((R(*)(Args...))func)(args...);
    }

    // template function that converts the specified value to a string using a stringstream
    template <typename T>
    std::string combine(T t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    // template function that converts the specified values to a string using a stringstream
    template <typename T, typename... Args>
    std::string combine(T t, Args... args)
    {
        std::stringstream ss;
        ss << t << combine(args...);
        return ss.str();
    }

    // template function that creates a hook at the specified target function with the provided detour function and stores the original function in the specified location
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

    // macro that checks if a value is within a range
    #define INRANGE(x, a, b) (x >= a && x <= b)

    // macro that converts two characters to a single byte
    #define GET_BYTE(x) ((GET_BITS(x[0]) << 4) | GET_BITS(x[1]))

    // macro that converts a character to a nibble (4 bits)
    #define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? (x - '0') : 0))

    // This function searches for a pattern of bytes within the memory range specified by range_start and range_end
    // The pattern to search for is specified as a string, where each byte in the pattern is represented by 2 hexadecimal characters
    // For example, the pattern "90 90 90 90" would represent the byte sequence 0x90 0x90 0x90 0x90
    // If the pattern is found within the specified memory range, a pointer to the start of the pattern is returned
    // Otherwise, a null pointer is returne
    static void* findSig(const char* sig) {
        // pointer to the current position in the pattern
        const char* pattern = sig;

        // store the first matched address
        uintptr_t first_match = 0;

        // store the first matched address
        static const uintptr_t range_start = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));

        // store information about the module
        static MODULEINFO mi_mod_info;

        // flag to initialize mi_mod_info only once
        static bool init = false;
        if (!init) {
            // initialize mi_mod_info
            init = true;
            GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(range_start), &mi_mod_info, sizeof(MODULEINFO));
        }
        // calculate the end address of the module
        static const uintptr_t range_end = range_start + mi_mod_info.SizeOfImage;

        // store the current byte in the pattern as a BYTE
        BYTE pat_byte = GET_BYTE(pattern);

        // store the original position in the pattern
        const char* old_pat = pattern;

        // iterate through all addresses in the module
        for (uintptr_t p_cur = range_start; p_cur < range_end; p_cur++) {
            // if we reached the end of the pattern, return the first matched address
            if (!*pattern) {
                return reinterpret_cast<void*>(first_match);
            }

            // skip spaces in the pattern
            while (*(PBYTE)pattern == ' ') {
                pattern++;
            }

            // if we reached the end of the pattern, return the first matched address
            if (!*pattern) {
                return reinterpret_cast<void*>(first_match);
            }

            // if the pattern has changed from the last iteration, update the pat_byte value
            if (old_pat != pattern) {
                old_pat = pattern;
                // if the current byte in the pattern is not a wildcard '?', update pat_byte to the current byte value
                if (*(PBYTE)pattern != '\?') {
                    pat_byte = GET_BYTE(pattern);
                }
            }

            // if the current byte in the pattern is a wildcard or if the current byte in the pattern matches the current byte in the range,
            // increment the pattern by 2 and set first_match to the current address if it is not already set
            if (*(PBYTE)pattern == '\?' || *(BYTE*)p_cur == pat_byte) {
                if (!first_match) {
                    first_match = p_cur;
                }

                // if the next byte in the pattern is the end, return the first matched address
                if (!pattern[2]) {
                    return reinterpret_cast<void*>(first_match);
                }

                pattern += 2;
            }
            else { // if the current byte in the pattern does not match the current byte in the range, reset the pattern to the start and clear first_match
                pattern = sig;
                first_match = 0;
            }
        }
    }
}