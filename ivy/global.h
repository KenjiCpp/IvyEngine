#pragma once

// Includes

#include <cassert>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

// Typedefs

typedef char                c8;
typedef char16_t            c16;
typedef char32_t            c32;

typedef bool                b8;

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef float               f32;
typedef double              f64;

typedef void*               ptr;
typedef const void*         cptr;

typedef std::type_info      type_info;
typedef std::type_index     type_index;

typedef std::mutex              mutex;
typedef std::thread             thread;
typedef std::condition_variable condition_variable;

template<typename T> using future      = std::future<T>;
template<typename T> using unique_lock = std::unique_lock<T>;
template<typename T> using init_list   = std::initializer_list<T>;

template<typename... Args> using tuple = std::tuple<Args...>;

template<typename F, typename... Args> using function = std::function<F(Args...)>;

typedef std::plus<>       plus;
typedef std::minus<>      minus;
typedef std::multiplies<> multiplies;
typedef std::divides<>    divides;
typedef std::negate<>     negate;

// Type traits

template<typename F, typename... Args> using result_type = std::result_of_t<F(Args...)>;

template<typename T> using remove_const_type           = std::remove_const_t<T>;
template<typename T> using remove_reference_type       = std::remove_reference_t<T>;
template<typename T> using remove_const_reference_type = std::remove_const_t<std::remove_reference_t<T>>;


// Import - export

#if defined(_DLL_BUILD)
	#define IVY_API __declspec(dllexport)
#else
	#define IVY_API __declspec(dllimport)
#endif


// Platform detection
// Source: https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define IVY_PLATFORM_WINDOWS
    #ifdef _WIN64
        #define IVY_PLATFORM_WINDOWS_64BIT
    #else
        #define IVY_PLATFORM_WINDOWS_32BIT
    #endif
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define IVY_PLATFORM_MACOS
    #endif
#elif __linux__
    #define IVY_PLATFORM_LINUX
#else
    #Error "Unknown compiler"
#endif