#include "ACProjectViewComponentMapInitializer.h"
#include <spark/SparkComponentFactory.h>

#include "ProjectMenu.h"
#include "ProjectImpl.h"
#include "ProjectViewerImpl.h"
#include "ContentImage.h"


namespace acprojectview {
    void ACProjectViewComponentMapInitializer::init() {
        spark::SparkComponentFactory::get().registerComponent("ProjectMenu", spark::create<ProjectMenu>);
        spark::SparkComponentFactory::get().registerComponent("ProjectImpl", spark::create<ProjectImpl>);
        spark::SparkComponentFactory::get().registerComponent("ProjectViewerImpl", spark::create<ProjectViewerImpl>);
        spark::SparkComponentFactory::get().registerComponent("ContentImage", spark::create<ContentImage>);
    }
}
