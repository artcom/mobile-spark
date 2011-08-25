/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// This file is part of the ART+COM Standard Library (asl).
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
//
// Description: TODO
//
// Last Review: NEVER, NOONE
//
//  review status report: (perfect, ok, fair, poor, disaster, notapplicable, unknown)
//    usefullness            : unknown
//    formatting             : unknown
//    documentation          : unknown
//    test coverage          : unknown
//    names                  : unknown
//    style guide conformance: unknown
//    technical soundness    : unknown
//    dead code              : unknown
//    readability            : unknown
//    understandabilty       : unknown
//    interfaces             : unknown
//    confidence             : unknown
//    integration            : unknown
//    dependencies           : unknown
//    cheesyness             : unknown
//
//    overall review status  : unknown
//
//    recommendations:
//       - unknown
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/
//
//    $RCSfile: Singleton.h,v $
//
//     $Author: christian $
//
//   $Revision: 1.5 $
//
// Description:
//
//=============================================================================

#ifndef _ac_mobile_masl_Singleton_h_included_
#define _ac_mobile_masl_Singleton_h_included_

#include "SingletonManager.h"

namespace masl {

template<class T>
class Singleton : public SingletonBase {
public:
	static T& get() {
		// A Singleton caches the instance returned by SingletonManager
		// in a local static variable.
		static T& myInstance(SingletonManager::get().SingletonManager::getSingletonInstance<T>());
		return myInstance;
	}
};

}

#endif
