#include "World.h"

#include "SparkComponentFactory.h"

namespace spark {

    const char * const World::SPARK_TYPE = "World";

    World::World(const BaseAppPtr theApp, const XMLNodePtr theXMLNode,
                   ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent){


    }

    World::~World() {
    }
}
