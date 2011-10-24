// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "DemoAppComponentMapInitializer.h"
#include <spark/SparkComponentFactory.h>
#include "Slide.h"

namespace demoapp {
    void DemoAppComponentMapInitializer::init() {
        spark::SparkComponentFactory::get().registerComponent(SlideImpl::SPARK_TYPE, spark::create<SlideImpl>);
    }
}
