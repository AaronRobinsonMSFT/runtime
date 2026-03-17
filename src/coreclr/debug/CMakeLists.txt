
add_compile_definitions($<${FEATURE_CORECLR_CACHED_INTERFACE_DISPATCH}:FEATURE_CACHED_INTERFACE_DISPATCH>)
add_compile_definitions($<${FEATURE_CORECLR_VIRTUAL_STUB_DISPATCH}:FEATURE_VIRTUAL_STUB_DISPATCH>)

include_directories(${RUNTIME_DIR})

if (NOT CLR_CMAKE_TARGET_ARCH_WASM)
    add_subdirectory(daccess)
endif ()
add_subdirectory(ee)
if (NOT CLR_CMAKE_TARGET_ARCH_WASM)
    add_subdirectory(di)
endif ()
if(CLR_CMAKE_HOST_WIN32)
    add_subdirectory(createdump)
endif(CLR_CMAKE_HOST_WIN32)
if(FEATURE_SINGLE_FILE_DIAGNOSTICS AND NOT CLR_CMAKE_TARGET_ARCH_WASM)
    add_subdirectory(runtimeinfo)
endif(FEATURE_SINGLE_FILE_DIAGNOSTICS AND NOT CLR_CMAKE_TARGET_ARCH_WASM)
