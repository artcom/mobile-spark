#ifndef _included_mobile_masl_Ptr_
#define _included_mobile_masl_Ptr_

#include <boost/shared_ptr.hpp>

#include "Exception.h"
#include "string_functions.h"

namespace masl {

#define PTR_DEBUG

#ifdef PTR_DEBUG
    
    DEFINE_EXCEPTION(NullPointerException, Exception)

    template <class T>
    class Ptr : public boost::shared_ptr<T> {
    public:
        Ptr();
        Ptr(T* theObject);
        T * operator->() const;
    };

    template <class T>
    Ptr<T>::Ptr() : boost::shared_ptr<T>() {
    }

    template <class T>
    Ptr<T>::Ptr(T* theObject) : boost::shared_ptr<T>(theObject) {
    }

    template <class T>
    T * Ptr<T>::operator->() const {
        T* myObject = boost::shared_ptr<T>::operator->();
        if (myObject) {
            return myObject;
        }
        throw NullPointerException("you are trying to access an object via a null pointer", PLUS_FILE_LINE);
    };

#else // PTR_DEBUG

    //#define Ptr boost::shared_ptr
    //typedef boost::shared_ptr Ptr

    template <class T>
    class Ptr : public boost::shared_ptr<T> {
    public:
        Ptr();
        Ptr(T* theObject);
    };

    template <class T>
    Ptr<T>::Ptr() : boost::shared_ptr<T>() {
    }

    template <class T>
    Ptr<T>::Ptr(T* theObject) : boost::shared_ptr<T>(theObject) {
    }
#endif // PTR_DEBUG

};

#endif  //include-guard
