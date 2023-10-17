#pragma once

// Declare a void pointer(Empty object) called __o__SendKey
void* __o__SendKey;

void keymapDetour(__int32 key, bool held) {
    // Set the value of Global::keymap[key] equal to held
    Global::keymap[key] = held;

    // Log the string "Key " concatenated with the value of key, a space, the string " ", and the value of held, followed by a newline character
    // Debug code that you can remove
    log(Utils::combine("Key ", key, " ", held , "\n").c_str());

    // Call the function at the address stored in __o__SendKey, passing in key and held as arguments
    Utils::CallFunc<void*, __int32, bool>(
        __o__SendKey,
        key,
        held
    );
}

class KeymapHook : public FuncHook {
public:
    bool Initialize() override
    {
        // Find the address of the function specified by the signature "48 83 EC 48 ? ? C1 4C" and store it in a void pointer called keymapAddr
        void* keymapAddr = Utils::findSig("48 83 EC 48 ? ? C1 4C");

        // Attempt to hook the function at the address stored in keymapAddr by replacing it with the address of keymapDetour and store the original function's address in __o__SendKey
        return Utils::HookFunction(
            keymapAddr,
            (void*)&keymapDetour,
            &__o__SendKey
        );
    }

    static KeymapHook& Instance() {
        static KeymapHook instance;
        return instance;
    }
};