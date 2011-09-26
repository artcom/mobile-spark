#include "World.h"

#include "SparkComponentFactory.h"

namespace spark {

    const char * const World::SPARK_TYPE = "World";

    World::World(const BaseAppPtr theApp, const XMLNodePtr theXMLNode):
        Widget(theApp, theXMLNode){


    }

    World::~World() {
    }
}
