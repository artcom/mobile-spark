#ifndef _included_mobile_spark_World_
#define _included_mobile_spark_World_

#include "Widget.h"

namespace spark {

    class World : public Widget {
    public:
        World(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~World();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return World::SPARK_TYPE;};
    private:
    };

    typedef boost::shared_ptr<World> WorldPtr;
};
#endif
