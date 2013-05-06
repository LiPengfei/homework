// File    : Global.h
// Module  : Public
// Filter  :
// Comment :

#ifndef __BLAZER_PUBLIC_GLOBAL_H__
#define __BLAZER_PUBLIC_GLOBAL_H__

#include "Macro.h"

/*--------------------------------- GLOBAL VARIABLE DEFINE ----------------------------------*/
#ifdef BZ_PLATFORM_OS_WINDOWS      // BZ_PLATFORM_OS_WINDOWS

    const int BZ_MAX_FILE_PATH_LEN = _MAX_PATH;                 // Include '\0' character
    const int BZ_MAX_FILE_NAME_LEN = _MAX_FNAME;                // Include '\0' character
    const int BZ_MAX_FILE_EXT_LEN  = _MAX_EXT;                  // Include '\0' character
    const int BZ_MAX_DIR_NAME_LEN  = _MAX_DIR;                  // Include '\0' character

#else                           // BZ_PLATFORM_OS_LINUX

    const int BZ_MAX_FILE_PATH_LEN = PATH_MAX;                  // Include '\0' character
    const int BZ_MAX_FILE_NAME_LEN = NAME_MAX + 1;              // Include '\0' character
    const int BZ_MAX_FILE_EXT_LEN  = NAME_MAX + 1;              // Include '\0' character
    const int BZ_MAX_DIR_NAME_LEN  = NAME_MAX + 1;              // Include '\0' character

#endif // BZ_PLATFORM_OS_WINDOWS

    const int BZ_MAX_IP_ADDR_LEN      = 16;                     // Include '\0' character
    const DWORD BZ_INVALID_ID_VALUE   = static_cast<DWORD>(-1); // Invalid number id of a string

/*----------------------------------- GLOBAL CLASS DEFINE -----------------------------------*/
class BUnCopyable
{
protected:
    BUnCopyable()  {}
    ~BUnCopyable() {}

private:
    BUnCopyable(const BUnCopyable &);
    BUnCopyable &operator=(const BUnCopyable &);
};

class BUnConstructable
{
private:
    BUnConstructable() {}
    BUnConstructable(const BUnConstructable &);
    BUnConstructable &operator=(const BUnConstructable &);

protected:
    ~BUnConstructable() {}
};

/*---------------------------------- GLOBAL FUNCTION DEFINE ---------------------------------*/
template <class T>
inline T BZ_Max(const T &lhs, const T &rhs)
{
    return lhs > rhs ? lhs : rhs;
}

template <class T>
inline T BZ_Min(const T &lhs, const T &rhs)
{
    return lhs > rhs ? rhs : lhs;
}

template <class T>
inline void BZ_Swap(T &lhs, T &rhs)
{
    T temp = lhs;           // copy   content
    lhs = rhs, rhs = temp;  // switch content
}

template <class T>
inline BYTE BZ_GetMemBYTE(const T * const p)
{
    return *((BYTE *)(p));
}

template <class T>
inline WORD BZ_GetMemWORD(const T * const p)
{
    return *((WORD *)(p));
}

template <class T>
inline DWORD BZ_GetMemDWORD(const T * const p)
{
    return *((DWORD *)(p));
}

template <class T>
inline void BZ_SafelyIncrement(T &value)
{
    value = ((value) + 1 > (value)) ? (value) + 1 : (value);
}

template<class T>
inline void BZ_ZeroMemory(T *p, size_t nMemSize)
{
    BZ_CHECK_POINTER_RETURN_VOID(p);
    ::memset((void *)p, 0, nMemSize);
}

/*------------------------------------------------------------------------------------------
    (1) When will an incomplete type of pointer occur? This situation happens when the 
declaration of a class is visible and the definition of a class is invisible.
    (2) Why we must check whether the type of the pointer is complete or not? The reason is
very simple - If we delete an incomplete pointer, the compiler will cause a warning "C4150",
delete a pointer to the incomplete type. When this happens, the destructor of the class will
not be invoked, then the resource of the object will be leaked. A simple example is like
below:
    // Deleter.h
    #pragma once

    class KToBeDeleted;
    class KDeleter
    {
    public:
        void Delete(KToBeDeleted *p);
    };

    // Deleter.cpp
    #include "deleter.h"

    void KDeleter::Delete(KToBeDeleted *p)
    {
        delete p;                // declaration is visible and definition is invisible
    }

    // ToBeDeleted.h
    #pragma once
    #include <stdio.h>

    class KToBeDeleted
    {
    public:
        ~KToBeDeleted()
        {
        ::printf("%s\n", "KToBeDeleted::~KToBeDeleted() invoked.");
        }
    };

    // main.cpp
    #include "Deleter.h"
    #include "ToBeDeleted.h"

    int main(int argc, char* argv[])
    {
        KToBeDeleted *p = ::new KToBeDeleted();
        KDeleter dtr;
        dtr.Delete(p);
        return 0;
    }
------------------------------------------------------------------------------------------*/
template <class T>
inline void BZ_SafelyDeletePtr(T *&p)
{
    // check if type is complete
    typedef char TypeMustBeComplete[ sizeof(T) ? 1 : -1 ];
    (void)sizeof(TypeMustBeComplete);

    if (NULL != p)
    {
        delete p; p = NULL;
    }
}

template <class T>
inline void BZ_SafelyDeleteArrayPtr(T *&p)
{
    // check if type is complete 
    typedef char TypeMustBeComplete[ sizeof(T) ? 1 : -1 ];
    (void)sizeof(TypeMustBeComplete);

    if (NULL != p)
    {
        delete [] p; p = NULL;
    }
}

template <class T>
inline void BZ_SafelyDeleteConstPtr(T *const p)
{
    // check if type is complete
    typedef char TypeMustBeComplete[ sizeof(T) ? 1 : -1 ];
    (void)sizeof(TypeMustBeComplete);

    if (NULL != p)
        delete p;
}

template <class T>
inline void BZ_SafelyDeleteConstArrayPtr(T *const p)
{
    // check if type is complete 
    typedef char TypeMustBeComplete[ sizeof(T) ? 1 : -1 ];
    (void)sizeof(TypeMustBeComplete);

    if (NULL != p)
        delete [] p;
}

// add by lipengfei 2013/04/24
template<class T>
inline void BZ_SetCode(T *pErrorCode, T code)
{
    if (NULL != pErrorCode)
        *pErrorCode = code;
}

// add end

template <class T>
inline void BZ_FreePtr(T *&p)
{
    if (NULL != p)
    {
        ::free(p); p = NULL;
    }
}

template <class T>
inline void BZ_FreeConstPtr(T *const p)
{
    if (NULL != p)
        ::free(p);
}

inline DWORD BZ_HashString2ID(const char * const cpcString)
{
    BZ_CHECK_C_STRING_RETURN_CODE(cpcString, BZ_INVALID_ID_VALUE);

    DWORD dwID = 0;
    for (int i = 0; '\0' != cpcString[i]; i++)
    {
        dwID = (dwID + (i + 1) * cpcString[i]) % 0x8000000B * 0xFFFFFFEF;
    }

    return (dwID ^ 0x12345678);
}


// add by lipengfei 2013/04/06
#ifdef _WIN32

#include <crtdbg.h>
inline void BZ_EnableMemLeakCheck()
{
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif
// add end

// add by lipengfei 2013/04/23
inline LONG BZ_AtomicIncrement(LONG &value)
{
#ifdef WIN32
#ifdef _MT
    return ::InterlockedIncrement(&value);
#else
    return ++value;
#endif
#else

#endif
}

inline LONG BZ_AtomicDecrement(LONG &value)
{
#ifdef WIN32
#ifdef _MT
    return ::InterlockedDecrement(&value);
#else
    return --value;
#endif
#else

#endif
}
// add end

// add by lipengfei 2013/04/25
inline void BZ_MilliSleep(DWORD dwMilliseconds)
{
#ifdef WIN32
    ::Sleep(dwMilliseconds);
#else
    ::usleep(dwMilliseconds * 1000);
#endif
}
// add end

#endif // __BLAZER_PUBLIC_GLOBAL_H__

