// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.


void SetSSFlag(DT_CONTEXT *, Thread *pThread)
{
    _ASSERTE(pThread != NULL);
    PORTABILITY_ASSERT("The function is not implemented on wasm");
}

void UnsetSSFlag(DT_CONTEXT *, Thread *pThread)
{
    _ASSERTE(pThread != NULL);
    PORTABILITY_ASSERT("The function is not implemented on wasm");
}

// Check if single stepping is enabled.
bool IsSSFlagEnabled(DT_CONTEXT *, Thread *pThread)
{
    _ASSERTE(pThread != NULL);
    PORTABILITY_ASSERT("The function is not implemented on wasm");
    return false;
}