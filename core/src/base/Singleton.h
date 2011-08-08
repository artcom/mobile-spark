/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// This file is part of the ART+COM Standard Library (asl).
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __*/
//
//
//
//=============================================================================

#ifndef _ac_mobile_asl_Singleton_h_included_
#define _ac_mobile_asl_Singleton_h_included_

namespace asl {

    template <class T>
    class Singleton {
      public:
         static T& get()
         {
            static T ourInstance ;
            return ourInstance ;
         }
    
      protected:
         Singleton(){}
    
      private:
         Singleton( const Singleton& ) ;
         Singleton& operator=( const Singleton& ) {return *this;}
    };
}

#endif
