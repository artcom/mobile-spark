#ifndef _included_mobile_masl_Ptr_
#define _included_mobile_masl_Ptr_

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include "Exception.h"
#include "string_functions.h"

namespace masl {

#define PTR_DEBUG

    DEFINE_EXCEPTION(NullPointerException, Exception)

    template <class T> class Ptr;

    template <class T>
    class WeakPtr : public boost::weak_ptr<T> {
    public:
        WeakPtr()                                           : boost::weak_ptr<T>() {}
        WeakPtr(const boost::weak_ptr<T>& r)                : boost::weak_ptr<T>(r) {}
        WeakPtr(const WeakPtr& r)                           : boost::weak_ptr<T>(r) {}
        WeakPtr(const Ptr<T>& r) : boost::weak_ptr<T>(r) {}

#ifdef PTR_DEBUG
        T * operator->() const;
#endif
    };

    template <class T>
    class Ptr : public boost::shared_ptr<T> {
    public:
        Ptr()                             : boost::shared_ptr<T>()  {};
        template<class Y> Ptr(boost::shared_ptr<Y> const& r): boost::shared_ptr<T>(r) {}
        template<class Y> explicit Ptr(Y* p)                : boost::shared_ptr<T>(p) {}
        template<class Y> Ptr(const Ptr<Y>& r)              : boost::shared_ptr<T>(r) {}
        template<class Y> explicit Ptr(WeakPtr<Y> const & r): boost::shared_ptr<T>(r) {}

#ifdef PTR_DEBUG
        T * operator->() const;
#endif
    };


#ifdef PTR_DEBUG
    template <class T>
    T * Ptr<T>::operator->() const {
        T* myObject = boost::shared_ptr<T>::operator->();
        if (myObject) {
            return myObject;
        }
        throw NullPointerException("Ptr: you are trying to access an object via a null pointer", PLUS_FILE_LINE);
    };

    template <class T>
    T * WeakPtr<T>::operator->() const {
        T* myObject = boost::weak_ptr<T>::operator->();
        if (myObject) {
            return myObject;
        }
        throw NullPointerException("WeakPtr: you are trying to access an object via a null pointer", PLUS_FILE_LINE);
    };
#endif
};

#endif  //include-guard
