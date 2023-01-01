#pragma once

// This is an abstract base class for function hooks to inherit
class FuncHook {
public:
    // This function initializes the hook
    virtual bool Initialize() = 0;
};

#include "Hooks/Keymap.h"

// This function initializes all registered function hooks
void InitializeHooks() {
    // This is an array of pointers to function hook objects
    static FuncHook* hooks[] = {
        &KeymapHook::Instance(),
    };

    // Iterate through all the hook objects
    for (std::size_t i = 0; i < std::size(hooks); ++i) {
        // Initialize the hook
        if (!hooks[i]->Initialize()) {
            // Handle initialization failure (Which for now we'll ignore)
        }
    }
}