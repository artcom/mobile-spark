// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "ACProjectViewComponentMapInitializer.h"
#include <spark/SparkComponentFactory.h>

#include "ProjectMenu.h"
#include "ProjectImpl.h"
#include "ProjectViewerImpl.h"
#include "ContentImage.h"
#include "AppLoaderAnim.h"
#include "MultiColumnText.h"


namespace acprojectview {
    void ACProjectViewComponentMapInitializer::init() {
        spark::SparkComponentFactory::get().registerComponent("ProjectMenu", spark::create<ProjectMenu>);
        spark::SparkComponentFactory::get().registerComponent("ProjectImpl", spark::create<ProjectImpl>);
        spark::SparkComponentFactory::get().registerComponent("ProjectViewerImpl", spark::create<ProjectViewerImpl>);
        spark::SparkComponentFactory::get().registerComponent("ContentImage", spark::create<ContentImage>);
        spark::SparkComponentFactory::get().registerComponent("AppLoaderAnim", spark::create<AppLoaderAnim>);
        spark::SparkComponentFactory::get().registerComponent("MultiColumnText", spark::create<MultiColumnText>);            
    }
}
