// File    : Platform.h
// Module  : Public
// Filter  :
// Comment :

#ifndef __BLAZER_PUBLIC_PLATFORM_H__
#define __BLAZER_PUBLIC_PLATFORM_H__

/*----------------------------------- SYSTEM MACRO DEFINE -----------------------------------*/
#ifdef WIN32                        // WIN32 Visual C++

    #define BZ_PLATFORM_NAME "windows"
    #define BZ_PLATFORM_OS_WINDOWS

    #ifdef _WIN64
        #define BZ_IN_64BIT_MODE
    #endif // _WIN64

#else                               // __GNUC__ GCC

    #define BZ_PLATFORM_NAME "linux"
    #define BZ_PLATFORM_OS_LINUX

    #ifdef __LP64__
        #define BZ_IN_64BIT_MODE
    #endif // __LP64__

#endif // WIN32

/*-------------------------------- COMMONLY USED HEADER FILE --------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #undef WIN32_LEAN_AND_MEAN
#else
    #include <unistd.h>
#endif // WIN32

#pragma warning(disable: 4996)

#endif // __BLAZER_PUBLIC_PLATFORM_H__
