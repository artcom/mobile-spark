#ifndef _included_mobile_spark_World_
#define _included_mobile_spark_World_

#include <masl/XMLNode.h>

#include "Widget.h"

namespace spark {
    
    class World : public Widget {
        public: 
            World(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent);
            virtual ~World();        
        private: 
    };

    typedef boost::shared_ptr<World> WorldPtr;
};
#endif
