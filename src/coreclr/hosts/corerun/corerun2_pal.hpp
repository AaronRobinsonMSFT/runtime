// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#ifndef __CORERUN_PAL_HPP__
#define __CORERUN_PAL_HPP__

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <functional>
#include <memory>

//
// Platform abstraction layer
//

namespace pal
{
    // Handle to a loaded module
    using mod_t = void*;

    struct free_delete
    {
        void operator()(void* x) { ::free(x); }
    };

    template<typename T>
    using malloc_ptr = std::unique_ptr<T, free_delete>;
}

#ifdef TARGET_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAIN __cdecl wmain
#define W(str) L ## str

namespace pal
{
    using char_t = wchar_t;
    using string_t = std::basic_string<char_t>;
    using stringstream_t = std::basic_stringstream<char_t>;
    using string_utf8_t = std::basic_string<char>;

    const char_t dir_delim = W('\\');
    const char_t env_path_delim = W(';');
    const char_t nativelib_ext[] = W(".dll");
    const char_t coreclr_lib[] = W("coreclr");

    int strcmp(const char_t* str1, const char_t* str2) { return wcscmp(str1, str2); }
    size_t strlen(const char_t* str) { return wcslen(str); }
    char_t* strdup(const char_t* str) { return ::_wcsdup(str); }
    int fprintf(FILE* fd, const char_t* const fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int ret = ::vfwprintf(fd, fmt, args);
        va_end(args);
        return ret;
    }
    string_t getenv(const char_t* var)
    {
        DWORD len = ::GetEnvironmentVariableW(var, nullptr, 0);
        if (len == 0)
            return {};

        malloc_ptr<char_t> buffer{ (char_t*)::malloc((len + 1) * sizeof(char_t)) };
        assert(buffer != nullptr);
        DWORD len2 = ::GetEnvironmentVariableW(var, buffer.get(), len);
        assert(len == len2 + 1);
        return { buffer.get() };
    }
    string_t get_exe_path()
    {
        char_t file_name[1024];
        DWORD count = ::GetModuleFileNameW(nullptr, file_name, ARRAYSIZE(file_name));
        assert(::GetLastError() != ERROR_INSUFFICIENT_BUFFER);

        return { file_name };
    }
    string_t get_absolute_path(const char_t* path)
    {
        DWORD needed = ::GetFullPathNameW(path, 0, nullptr, nullptr);
        malloc_ptr<char_t> buffer{ (char_t*)::malloc(needed * sizeof(char_t)) };
        assert(buffer != nullptr);

        DWORD wrote = ::GetFullPathNameW(path, needed, buffer.get(), nullptr);
        assert(needed == wrote + 1);
        return { buffer.get() };
    }

    bool is_debugger_attached() { return ::IsDebuggerPresent() == TRUE; }

    bool does_file_exist(const string_t& file_path)
    {
        return INVALID_FILE_ATTRIBUTES != ::GetFileAttributesW(file_path.c_str());
    }

    pal::string_t build_file_list(
        const pal::string_t& dir,
        const char_t* ext,
        std::function<bool(const char_t*)> should_add)
    {
        assert(ext != nullptr);

        pal::string_t dir_local = dir;
        dir_local.append(W("*"));
        dir_local.append(ext);

        WIN32_FIND_DATA data;
        HANDLE findHandle = ::FindFirstFileW(dir_local.data(), &data);
        if (findHandle == INVALID_HANDLE_VALUE)
            return {};

        stringstream_t file_list;
        do
        {
            if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // ToLower for case-insensitive comparisons
                char_t* fileNameChar = data.cFileName;
                while (*fileNameChar)
                {
                    *fileNameChar = towlower(*fileNameChar);
                    fileNameChar++;
                }

                if (should_add(data.cFileName))
                {
                    file_list << dir << data.cFileName << pal::env_path_delim;
                }
                else
                {
                    //*m_log << W("Not adding ") << targetPath << data.cFileName << W(" to the TPA list because another file with the same name is already present on the list") << Logger::endl;
                }
            }
        } while (FALSE != ::FindNextFileW(findHandle, &data));

        ::FindClose(findHandle);

        return file_list.str();
    }

    void* get_module_symbol(mod_t m, const char* sym)
    {
        assert(m != nullptr && sym != nullptr);
        return ::GetProcAddress((HMODULE)m, sym);
    }

    string_utf8_t convert_to_utf8(const char_t* str)
    {
        // Compute the needed buffer
        int bytes_req = ::WideCharToMultiByte(
            CP_UTF8, 0, // Conversion args
            str, -1,    // Input string
            nullptr, 0, // Null to request side
            nullptr, nullptr);

        malloc_ptr<char> buffer{ (char*)::malloc(bytes_req) };
        assert(buffer != nullptr);

        int written = ::WideCharToMultiByte(
            CP_UTF8, 0, // Conversion args
            str, -1,    // Input string
            buffer.get(), bytes_req,  // Output buffer
            nullptr, nullptr);
        assert(bytes_req == written);

        return { buffer.get() };
    }

    string_utf8_t convert_to_utf8(string_t&& str)
    {
        return convert_to_utf8(str.c_str());
    }
}

#else // !TARGET_WINDOWS
#include <getexepath.h>

#define MAIN main
#define W(str) str
#define FAILED(result) (result < 0)

namespace pal
{
    using char_t = char;
    using string_t = std::basic_string<char_t>;
    using stringstream_t = std::basic_stringstream<char_t>;
    using utf8_t = std::basic_string<char>;

    const char_t dir_delim = W('/');
    const char_t env_path_delim = W(':');

#if defined(__APPLE__)
    const char_t nativelib_ext[] = W(".dylib");
#else // Various Linux-related OS-es
    const char_t nativelib_ext[] = W(".so");
#endif
    const char_t coreclr_lib[] = W("libcoreclr");

    int strcmp(const char_t* str1, const char_t* str2) { return ::strcmp(str1, str2); }
    size_t strlen(const char_t* str) { return ::strlen(str); }
    char_t* strdup(const char_t* str) { return ::strdup(str); }
    int fprintf(FILE* fd, const char_t* const fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int ret = ::vfprintf(fd, fmt, args);
        va_end(args);
        return ret;
    }
    char_t* getenv(const char_t* var)
    {
        char_t* val = ::getenv(var);
        if (val == nullptr)
            return{};
        return { val };
    }

    char_t* get_exe_path() { return getexepath(); }

    string_t get_absolute_path(const string_t& path)
    {
        char realPath[PATH_MAX];
        if (realpath(path, realPath) != nullptr && realPath[0] != '\0')
        {
            absolutePath.assign(realPath);
            // realpath should return canonicalized path without the trailing slash
            assert(absolutePath.back() != '/');

            result = true;
        }
    }

https://developer.apple.com/library/archive/qa/qa1361/_index.html
    bool is_debugger_attached();

    bool does_file_exist(const char_t* file_path)
    {
        // Check if the specified path exists
        struct stat sb;
        if (stat(file_path, &sb) == -1)
        {
            perror(W("Path not found"));
            return false;
        }

        // Verify that the path points to a file
        if (!S_ISREG(sb.st_mode))
        {
            fprintf(stderr, W("The specified managed assembly is not a file: %s\n"), file_path);
            return false;
        }

        return true;
    }

    pal::string_t build_file_list(
        const pal::string_t& dir,
        const char_t* ext,
        std::function<bool(const char_t*)> should_add);

    void* get_module_symbol(mod_t m, const char* sym);

    string_utf8_t convert_to_utf8(const char_t* str)
    {
        return { str };
    }

    string_utf8_t convert_to_utf8(string_t&& str)
    {
        return str;
    }
}

#endif // !TARGET_WINDOWS

namespace pal
{
    void split_path_to_dir_filename(const pal::string_t& path, pal::string_t& dir, pal::string_t& filename)
    {
        size_t pos = path.find_last_of(dir_delim);
        if (pos == pal::string_t::npos)
        {
            dir = {};
            filename = path;
            return;
        }

        dir = path.substr(0, pos);
        filename = path.substr(pos + 1);
    }

    bool string_ends_with(const string_t& str, size_t suffix_len, const char_t* suffix)
    {
        assert(suffix != nullptr);

        size_t str_len = str.length();
        if (str_len < suffix_len)
            return false;

        const char_t* suffix_maybe = str.data() + (str_len - suffix_len);
        return ::memcmp(suffix_maybe, suffix, suffix_len * sizeof(char_t)) == 0;
    }

    template<size_t LEN>
    bool string_ends_with(const string_t& str, const char_t(&suffix)[LEN])
    {
        return string_ends_with(str, LEN - 1, suffix);
    }

    void ensure_trailing_delimiter(pal::string_t& dir)
    {
        if (dir.empty())
        {
            dir = pal::dir_delim;
        }
        else if (dir.back() != pal::dir_delim)
        {
            dir.push_back(pal::dir_delim);
        }
    }

    const char** convert_argv_to_utf8(int argc, const char_t** argv, std::vector<string_utf8_t>& lifetime)
    {
        malloc_ptr<const char*> ret{ (const char**)::malloc(sizeof(char*) * argc) };
        assert(ret != nullptr);

        for (int i = 0; i < argc; ++i)
        {
            string_utf8_t s = convert_to_utf8(argv[i]);
            lifetime.push_back(std::move(s));
            ret.get()[i] = lifetime.back().c_str();
        }

        return ret.release();
    }
}

#endif // __CORERUN_PAL_HPP__
