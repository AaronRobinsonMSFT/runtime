// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace System.Runtime.CompilerServices
{
    /// <summary>
    /// Helpers for UnsafeAccessor runtime support.
    /// </summary>
    [StackTraceHidden]
    [DebuggerStepThrough]
    internal static unsafe partial class UnsafeAccessorHelpers
    {
#pragma warning disable IDE0060
        [MethodImpl(MethodImplOptions.NoInlining)]
        internal static void VerifyConstraintsAtRuntime(RuntimeTypeHandle targetType, RuntimeMethodHandle targetMethod)
        {
        }
#pragma warning restore IDE0060
    }
}
