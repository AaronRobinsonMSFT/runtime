// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

import type { RuntimeAPI } from "./types";
import { _ems_ } from "../../Common/JavaScript/ems-ambient";
import { stringToUTF8Ptr } from "./strings";

export function registerDebugger(runtimeApi: RuntimeAPI): void {
    runtimeApi.INTERNAL.DotNetOnDebuggerMessage = (message: string) => {
        const ptr = stringToUTF8Ptr(message);
        try {
            _ems_._DotNetOnDebuggerMessage(ptr);
        } finally {
            _ems_._free(ptr as any);
        }
    };
}
