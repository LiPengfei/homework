// File    : Macro.h
// Module  : Public
// Filter  :
// Comment :

#ifndef __BLAZER_PUBLIC_MACRO_H__
#define __BLAZER_PUBLIC_MACRO_H__

#include "Platform.h"

#ifdef BZ_USING_NAMESPACE

    #define BZ_DECLARE_NAMESPACE_BEGIN(module) \
        namespace blazer_##module {
    #define BZ_DECLARE_NAMESPACE_END }
    #define BZ(module) blazer_##module

#else

    #define BZ_DECLARE_NAMESPACE_BEGIN(module)
    #define BZ_DECLARE_NAMESPACE_END
    #define BZ(module)

#endif // BZ_USING_NAMESPACE

#define BZ_SABRE  BZ(sabre)
#define BZ_ENGINE BZ(engine)

#ifdef __cplusplus              // C++

    #ifndef NULL
    #define NULL        0
    #endif // NULL

#else                           // C

    #ifndef NULL
    #define NULL        ((void*)0)
    #endif // NULL

#endif // __cplusplus

#ifndef INOUT
#define INOUT
#endif // INOUT

#ifdef BZ_PLATFORM_OS_WINDOWS      // BZ_PLATFORM_OS_WINDOWS

#else                              // BZ_PLATFORM_OS_LINUX
    #define TRUE    1
    #define FALSE   0

    #define IN
    #define OUT

    #define UNREFERENCED_PARAMETER(P) (void)(P)
#endif // BZ_PLATFORM_OS_WINDOWS

/*------------------------------------------------------------------------------------------
    "__pragma(warning(push))"                 means  "#pragma warning(push)"                
    "__pragma(warning(disable:warning_code))" means  "#pragma warning(disable:warning_code)"
    "__pragma(warning(pop))"                  means  "#pragma warning(pop)"                 
                                                                                            
    MS VC++ 11.0    Visual C++ 2012     _MSC_VER = 1700                                     
    MS VC++ 10.0    Visual C++ 2010     _MSC_VER = 1600                                     
    MS VC++ 9.0     Visual C++ 2008     _MSC_VER = 1500                                     
    MS VC++ 8.0     Visual C++ 2005     _MSC_VER = 1400                                     
    MS VC++ 7.1     Visual C++ 2003     _MSC_VER = 1310                                     
    MS VC++ 7.0                         _MSC_VER = 1300                                     
    MS VC++ 6.0                         _MSC_VER = 1200                                     
    MS VC++ 5.0                         _MSC_VER = 1100                                     
------------------------------------------------------------------------------------------*/
#undef BZ_DISABLE_WARNING
#ifdef _MSC_VER                 // MS VC++
    #define BZ_DISABLE_WARNING(warning_code, expression) \
        __pragma(warning(push)) \
        __pragma(warning(disable:warning_code)) expression \
        __pragma(warning(pop))
#else                           // GCC
    #define BZ_DISABLE_WARNING(warning_code, expression) \
        expression
#endif // _MSC_VER

#undef  BZ_WHILE_FALSE_NO_WARNING
#define BZ_WHILE_FALSE_NO_WARNING BZ_DISABLE_WARNING(4127, while(FALSE))

#undef BZ_ASSERT_FALSE
#define BZ_ASSERT_FALSE(condition)  \
        assert(FALSE && #condition)

// assert MACROS
// Macro : BZ_PROCESS_ERROR
#undef  BZ_PROCESS_ERROR
#define BZ_PROCESS_ERROR(condition) \
    do \
    { \
        if (!(condition)) \
        { \
            BZ_ASSERT_FALSE(condition); \
            goto Exit0; \
        } \
    } BZ_WHILE_FALSE_NO_WARNING

// Macro : BZ_CHECK_RETURN_CODE
#undef  BZ_CHECK_RETURN_CODE
#define BZ_CHECK_RETURN_CODE(condition, ret_code) \
    do \
    { \
        if (!(condition)) \
        { \
            BZ_ASSERT_FALSE(condition); \
            return ret_code; \
        } \
    } BZ_WHILE_FALSE_NO_WARNING

// Macro : BZ_CHECK_RETURN_BOOL
#undef  BZ_CHECK_RETURN_BOOL
#define BZ_CHECK_RETURN_BOOL(condition) \
    BZ_CHECK_RETURN_CODE(condition, FALSE)

// Macro : BZ_CHECK_RETURN_VOID
#undef  BZ_CHECK_RETURN_VOID
#define BZ_CHECK_RETURN_VOID(condition) \
    do \
    { \
        if (!(condition)) \
        { \
            BZ_ASSERT_FALSE(condition); \
            return ; \
        } \
    } BZ_WHILE_FALSE_NO_WARNING

// Macro : BZ_CHECK_C_STRING_RETURN_BOOL
#undef  BZ_CHECK_C_STRING_RETURN_BOOL
#define BZ_CHECK_C_STRING_RETURN_BOOL(s) \
    BZ_CHECK_RETURN_BOOL(NULL != s && '\0' != s[0])

// Macro : BZ_CHECK_C_STRING_RETURN_VOID
#undef  BZ_CHECK_C_STRING_RETURN_VOID
#define BZ_CHECK_C_STRING_RETURN_VOID(s) \
    BZ_CHECK_RETURN_VOID(NULL != s && '\0' != s[0])

// Macro : BZ_CHECK_C_STRING_RETURN_CODE
#undef  BZ_CHECK_C_STRING_RETURN_CODE
#define BZ_CHECK_C_STRING_RETURN_CODE(s, ret_code) \
    BZ_CHECK_RETURN_CODE((NULL != s && '\0' != s[0]), ret_code)

// Macro : BZ_CHECK_POINTER_RETURN_BOOL
#undef  BZ_CHECK_POINTER_RETURN_BOOL
#define BZ_CHECK_POINTER_RETURN_BOOL(p) \
    BZ_CHECK_RETURN_BOOL(NULL != p)

// Macro : BZ_CHECK_POINTER_RETURN_VOID
#undef  BZ_CHECK_POINTER_RETURN_VOID
#define BZ_CHECK_POINTER_RETURN_VOID(p) \
    BZ_CHECK_RETURN_VOID(NULL != p)

// Macro : BZ_CHECK_POINTER_RETURN_CODE
#undef  BZ_CHECK_POINTER_RETURN_CODE
#define BZ_CHECK_POINTER_RETURN_CODE(p, ret_code) \
    BZ_CHECK_RETURN_CODE(NULL != p, ret_code)

// QUIET MACROS
// Macro : BZ_PROCESS_ERROR_QUIET
#undef  BZ_PROCESS_ERROR_QUIET
#define BZ_PROCESS_ERROR_QUIET(condition) \
    do \
    { \
        if (!(condition)) \
        { \
            goto Exit0; \
        } \
    } BZ_WHILE_FALSE_NO_WARNING

// Macro : BZ_CHECK_RETURN_CODE_QUIET
#undef  BZ_CHECK_RETURN_CODE_QUIET
#define BZ_CHECK_RETURN_CODE_QUIET(condition, ret_code) \
    do \
    { \
        if (!(condition)) \
        { \
            return ret_code; \
        } \
    } BZ_WHILE_FALSE_NO_WARNING

// Macro : BZ_CHECK_RETURN_BOOL_QUIET
#undef  BZ_CHECK_RETURN_BOOL_QUIET
#define BZ_CHECK_RETURN_BOOL_QUIET(condition) \
    BZ_CHECK_RETURN_CODE_QUIET(condition, FALSE)

// Macro : BZ_CHECK_RETURN_VOID_QUIET
#undef  BZ_CHECK_RETURN_VOID_QUIET
#define BZ_CHECK_RETURN_VOID_QUIET(condition) \
    do \
    { \
        if (!(condition)) \
        { \
            return ; \
        } \
    } BZ_WHILE_FALSE_NO_WARNING

// Macro : BZ_CHECK_C_STRING_RETURN_BOOL_QUIET
#undef  BZ_CHECK_C_STRING_RETURN_BOOL_QUIET
#define BZ_CHECK_C_STRING_RETURN_BOOL_QUIET(s) \
    BZ_CHECK_RETURN_BOOL_QUIET(NULL != s && '\0' != s[0])

// Macro : BZ_CHECK_C_STRING_RETURN_VOID_QUIET
#undef  BZ_CHECK_C_STRING_RETURN_VOID_QUIET
#define BZ_CHECK_C_STRING_RETURN_VOID_QUIET(s) \
    BZ_CHECK_RETURN_VOID_QUIET(NULL != s && '\0' != s[0])

// Macro : BZ_CHECK_C_STRING_RETURN_CODE_QUIET
#undef  BZ_CHECK_C_STRING_RETURN_CODE_QUIET
#define BZ_CHECK_C_STRING_RETURN_CODE_QUIET(s, ret_code) \
    BZ_CHECK_RETURN_CODE_QUIET((NULL != s && '\0' != s[0]), ret_code)

// Macro : BZ_CHECK_POINTER_RETURN_BOOL_QUIET
#undef  BZ_CHECK_POINTER_RETURN_BOOL_QUIET
#define BZ_CHECK_POINTER_RETURN_BOOL_QUIET(p) \
    BZ_CHECK_RETURN_BOOL_QUIET(NULL != p)

// Macro : BZ_CHECK_POINTER_RETURN_VOID_QUIET
#undef  BZ_CHECK_POINTER_RETURN_VOID_QUIET
#define BZ_CHECK_POINTER_RETURN_VOID_QUIET(p) \
    BZ_CHECK_RETURN_VOID_QUIET(NULL != p)

// Macro : BZ_CHECK_POINTER_RETURN_CODE_QUIET
#undef  BZ_CHECK_POINTER_RETURN_CODE_QUIET
#define BZ_CHECK_POINTER_RETURN_CODE_QUIET(p, ret_code) \
    BZ_CHECK_RETURN_CODE_QUIET(NULL != p, ret_code)

// Macro       : BZ_FETCH_FIELD_SIZE
// Description : fetch the size of field in a struct or class.
// Param       : type : class or struct name
//               field : member variable name
#define BZ_FETCH_FIELD_SIZE(type, field) \
    sizeof(((type *)0)->field)

// Macro       : BZ_FETCH_FIELD_OFFSET
// Description : fetch the offset of field in a struct or class.
// Param       : type : class or struct name
//               field : member variable name
#define BZ_FETCH_FIELD_OFFSET(type, field) \
    ((ULONG_PTR)&((type *)0)->field)

// Macro       : BZ_FETCH_ADDRESS_BY_FIELD
// Description : fetch the size of class or struct by member variable address
// Param       : address : the address of member variable instance
//               type    : class or struct name
//               field   : member variable name
#ifdef BZ_PLATFORM_OS_WINDOWS      // PLATFORM_OS_WINDOWS
    #define BZ_FETCH_ADDRESS_BY_FIELD(address, type, field) \
        CONTAINING_RECORD(address, type, field)
#else                           // PLATFORM_OS_LINUX
    //#define BZ_FETCH_ADDRESS_BY_FIELD(address, type, field) \
    //    ((type *)((char *)(address) - (ULONG_PTR)(&((type *)0)->field)))

    #define BZ_FETCH_ADDRESS_BY_FIELD(address, type, field) \
        ((type *)((char *)(address) - BZ_FETCH_FIELD_OFFSET(type, field)))
#endif

#define BZ_DEBUG_PRINT_INT(expression)                          \
    printf("[DEBUG PRINT] Expression \"%s\" returned: %d\n",    \
    (#expression), (expression))

#define BZ_DEBUG_PRINT_BOOL(expression)                         \
    printf("[DEBUG PRINT] Expression \"%s\" returned: %s\n",    \
    (#expression), (expression) == TRUE ? "TRUE" : "FALSE")

// add by lipengfei 2013/04/26

// #define BZ_CHECK_SOCKET_RETURN_BOOL(sock)            \
//     do {                                             \
//         if (INVALID_SOCKET == (sock) || SOCKET_ERROR == (sock)) \
//             BZ_CHECK_RETURN_BOOL(FALSE);             \ 
//     } BZ_WHILE_FALSE_NO_WARNING

#define BZ_CHECK_SOCKET_RETURN_ERRCODE(sock)        \
    do { \
        if (INVALID_SOCKET == (sock) || SOCKET_ERROR == (sock))    \
        {                                               \
            DWORD code = ::WSAGetLastError();           \
            BZ_CHECK_RETURN_CODE(FALSE, code);          \
        } \
    } BZ_WHILE_FALSE_NO_WARNING


#define BZ_CHECK_SOCKET_RETURN_CODE(sock, code)  \
    do { \
        if (INVALID_SOCKET == (sock) || SOCKET_ERROR == (sock)) \
            BZ_CHECK_RETURN_CODE(FALSE, code);       \
    } BZ_WHILE_FALSE_NO_WARNING

#define BZ_CHECK_SOCKET_RETURN_BOOL_QUIET(sock)  \
    do { \
        if (INVALID_SOCKET == (sock) || SOCKET_ERROR == (sock)) \
            BZ_CHECK_RETURN_BOOL_QUIET(FALSE); \
    } BZ_WHILE_FALSE_NO_WARNING

#define BZ_CHECK_SOCKET_RETURN_ERRCODE_QUIET(sock) \
    do { \
        if (INVALID_SOCKET == (sock) || SOCKET_ERROR == (sock))   \
        {                                              \
            DWORD code = ::WSAGetLastError();          \
            BZ_CHECK_RETURN_CODE_QUIET(FALSE, code);   \
        } \
    } BZ_WHILE_FALSE_NO_WARNING

#define BZ_CHECK_SOCKET_RETURN_CODE_QUIET(sock, code)   \
    do { \
        if (INVALID_SOCKET == (sock) || 0 == (sock))        \
            BZ_CHECK_RETURN_CODE_QUIET(FALSE, code);        \
    } BZ_WHILE_FALSE_NO_WARNING
// add end

#endif // __BLAZER_PUBLIC_MACRO_H__
