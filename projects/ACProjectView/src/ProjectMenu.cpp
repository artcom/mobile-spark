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
}
