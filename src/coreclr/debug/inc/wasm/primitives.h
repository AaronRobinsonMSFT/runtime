// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//*****************************************************************************
// File: primitives.h
//

//
// Platform-specific debugger primitives
//
//*****************************************************************************

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

inline CORDB_ADDRESS GetPatchEndAddr(CORDB_ADDRESS patchAddr)
{
    PORTABILITY_ASSERT("The function is not implemented on wasm");
    return patchAddr;
}

typedef const BYTE                  CORDB_ADDRESS_TYPE;
typedef DPTR(CORDB_ADDRESS_TYPE)    PTR_CORDB_ADDRESS_TYPE;

//This is an abstraction to keep x86/ia64 patch data separate
#define PRD_TYPE                               USHORT

#define MAX_INSTRUCTION_LENGTH 2 // update once we have codegen

#define CORDbg_BREAK_INSTRUCTION_SIZE 1
#define CORDbg_BREAK_INSTRUCTION 0 // unreachable intruction

// Enable single stepping.
void SetSSFlag(DT_CONTEXT *pCtx, Thread *pThread);

// Disable single stepping
void UnsetSSFlag(DT_CONTEXT *pCtx, Thread *pThread);

// Check if single stepping is enabled.
bool IsSSFlagEnabled(DT_CONTEXT *pCtx, Thread *pThread);

inline void InitializePRD(PRD_TYPE *p1)
{
    *p1 = 0;
}

inline bool PRDIsEmpty(PRD_TYPE p1)
{
    LIMITED_METHOD_CONTRACT;

    return p1 == 0;
}

inline LPVOID CORDbgGetIP(DT_CONTEXT *context)
{
    LIMITED_METHOD_CONTRACT;
    PORTABILITY_ASSERT("The function is not implemented on wasm");
    return NULL;
}

inline void CORDbgSetInstruction(CORDB_ADDRESS_TYPE* address,
                                 PRD_TYPE instruction)
{
    LIMITED_METHOD_DAC_CONTRACT;
    PORTABILITY_ASSERT("The function is not implemented on wasm");
}

inline PRD_TYPE CORDbgGetInstruction(UNALIGNED CORDB_ADDRESS_TYPE* address)
{
    LIMITED_METHOD_CONTRACT;
    PORTABILITY_ASSERT("The function is not implemented on wasm");
    TADDR ptraddr = dac_cast<TADDR>(address);
    return *(PRD_TYPE *)ptraddr;
}

inline bool AddressIsBreakpoint(CORDB_ADDRESS_TYPE* address)
{
    LIMITED_METHOD_CONTRACT;
    PORTABILITY_ASSERT("The function is not implemented on wasm");
    return false;
}

inline void CORDbgInsertBreakpoint(UNALIGNED CORDB_ADDRESS_TYPE *address)
{
    LIMITED_METHOD_CONTRACT;
    PORTABILITY_ASSERT("The function is not implemented on wasm");
}

#endif
