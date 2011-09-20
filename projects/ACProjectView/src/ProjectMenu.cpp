#include "ProjectMenu.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace acprojectview {

    ProjectMenu::ProjectMenu(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent)
        : Transform(theApp, theXMLNode, theParent) {
    }

    ProjectMenu::~ProjectMenu() {}

    void
    ProjectMenu::arrangeProjects(const int theWidth, const int theHeight) {
    }
    
    int ProjectMenu::getPreviewWidth() {
        return 200;
    }
    
    int ProjectMenu::getPreviewHeight(){
        return 200;
    }
    
}
