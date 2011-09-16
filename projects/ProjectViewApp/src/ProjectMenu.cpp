#include "ProjectMenu.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace projectviewapp {

    //needed for component factory
    namespace  {
        const bool registered = spark::SparkComponentFactory::get().registerComponent("ProjectMenu", spark::create<ProjectMenu>);
    }
    
    ProjectMenu::ProjectMenu(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Transform(theApp, theXMLNode, theParent) {
        AC_PRINT << "##################################### PROJECTMENU KONSTRUKTOR";
    }

    ProjectMenu::~ProjectMenu() {}


    void
    ProjectMenu::arrangeProjects(const int theWidth, const int theHeight) {
    }
}
