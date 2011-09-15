#include "World.h"

#include "SparkComponentFactory.h"

namespace spark {

    const char * World::SPARK_TYPE = "World";

    World::World(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent){
            
            
    }

    World::~World() {
    }        
}
