#ifndef __BLASER_SABRE_SINGLETON_H__
#define __BLASER_SABRE_SINGLETON_H__ 

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

template <typename T>
class BSingleton : private BUnCopyable
{
public:
    static T& GetInstance()
    {
        static T l_sInstance;
        return l_sInstance;
    }
};

#define DECLARE_SINGLETON_PATTERN(T) friend class BZ_SABRE::BSingleton<T>
#define SINGLETON_GET_REF(T)         (BZ_SABRE::BSingleton<T>::GetInstance())
#define SINGLETON_GET_PTR(T)         (&BZ_SABRE::BSingleton<T>::GetInstance())

BZ_DECLARE_NAMESPACE_END

#endif
