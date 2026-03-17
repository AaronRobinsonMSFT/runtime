// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

// This file provides the WASM-exported entry point for debugger messages
// received via Chrome DevTools Protocol (CDP). The external debugger tool
// calls DotNetOnDebuggerMessage through a CDP Runtime.evaluate invocation,
// which is wired through the JavaScript/TypeScript layer.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

extern "C" EMSCRIPTEN_KEEPALIVE void DotNetOnDebuggerMessage(const char* message)
{
    // TODO: Process the incoming debugger message.
    // This should enter the debugger message loop and trigger a break.
}
#endif // __EMSCRIPTEN__
