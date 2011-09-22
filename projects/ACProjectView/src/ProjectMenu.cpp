#include "ProjectMenu.h"
#include <spark/SparkComponentFactory.h>

using namespace spark;

namespace acprojectview {

    ProjectMenu::ProjectMenu(const BaseAppPtr theApp, const XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode) {
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
