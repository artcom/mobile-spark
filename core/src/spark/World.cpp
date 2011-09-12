#include "World.h"

#include "SparkComponentFactory.h"

namespace spark {

    //needed for component factory
    namespace  {
        const bool registered = spark::SparkComponentFactory::get().registerComponent("World", spark::create<World>);
    }


    World::World(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent){
            
            
    }

    World::~World() {
    }        
}
