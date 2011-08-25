#include "World.h"

#include "SparkComponentFactory.h"

namespace spark {

    //needed for component factory
    //namespace  {
        ComponentPtr createWorld(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent) {
            return WorldPtr(new World(theApp, theXMLNode, theParent));
        };
        //const bool registered = spark::SparkComponentFactory::get().registerComponent("Window", spark::createWindow);
    //}


    World::World(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, 
                   ComponentPtr theParent):
        Widget(theApp, theXMLNode, theParent){
    }

    World::~World() {
    }        
}
