#pragma once

#include "../../Memory/Structs/MinecraftUIRenderContext.h"

// Declare a void pointer called __o__Render
void* __o__Render;

// Declare a variable called layerCounter and initialize it to 0
int layerCounter = 0;

// Call the function at the address stored in __o__Render, passing in __this and ctx as arguments
void renderDetour(void* __this, MinecraftUIRenderContext* ctx) {
    // Call the function at the address stored in __o__Render, passing in __this and ctx as arguments
    Utils::CallFunc<void*, void*, MinecraftUIRenderContext*>(
        __o__Render,
        __this,
        ctx
    );

    // Increment layerCounter
    layerCounter++;

    // If layerCounter is equal to 3
    if (layerCounter == 3) {
        // Set layerCounter to 0
        layerCounter = 0;

        // Declare a UIColor object called color and initialize it with the values 255, 255, 255, 255
        UIColor color = UIColor{ 255, 255, 255, 255 };

        // Declare a Vector4 object called rectPos and initialize it with the values { 25, 25 }, { 25, 25 }
        Vector4<float> rectPos({ 25, 25 }, { 25, 25 });

        // Call the fillRectangle function on ctx, passing in rectPos and color as arguments
        ctx->fillRectangle(rectPos, color, color.a);
    }
}

class RenderContextHook : public FuncHook {
public:
    bool Initialize() override
    {
        // 2873 is the size of the machine code (Excluding variables) at the time im making this

        // Find the address of the function specified by the signature {SIG} and store it in a void pointer called renderAddr
        void* renderAddr = Utils::findSig("48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? B8 0F 29 ? A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B F2 48 89 54 24 ? 4C");

        // Attempt to hook the function at the address stored in renderAddr by replacing it with the address of renderDetour and store the original function's address in __o__Render
        return Utils::HookFunction(
            renderAddr,
            (void*)&renderDetour,
            &__o__Render
        );
    }

    static RenderContextHook& Instance() {
        static RenderContextHook instance;
        return instance;
    }
};