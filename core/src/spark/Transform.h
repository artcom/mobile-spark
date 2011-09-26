#ifndef _included_mobile_spark_Transform_
#define _included_mobile_spark_Transform_

#include "Widget.h"

namespace spark {

    class Transform : public Widget {
    public:
        Transform(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Transform();
        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Transform::SPARK_TYPE;};
    private:
    };

    typedef boost::shared_ptr<Transform> TransformPtr;
};

#endif
