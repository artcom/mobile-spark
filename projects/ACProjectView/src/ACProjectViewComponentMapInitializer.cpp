#include "ACProjectViewComponentMapInitializer.h"
#include <spark/SparkComponentFactory.h>

#include "ProjectMenu.h"
#include "ProjectMenuItemImpl.h"

namespace acprojectview {
    void ACProjectViewComponentMapInitializer::init() {
        spark::SparkComponentFactory::get().registerComponent("ProjectMenu", spark::create<ProjectMenu>);
        spark::SparkComponentFactory::get().registerComponent("ProjectMenuItemImpl", spark::create<ProjectMenuItemImpl>);
    }
}
