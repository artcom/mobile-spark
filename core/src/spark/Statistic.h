#ifndef _included_mobile_spark_Statistic_
#define _included_mobile_spark_Statistic_

#include "Transform.h"
#include "Text.h"

namespace spark {

    class Statistic : public Transform {
    public: 
        Statistic(const BaseAppPtr theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent = ComponentPtr());
        virtual ~Statistic();    
        virtual void prerender(MatrixStack& theCurrentMatrixStack);   
        virtual void onPause();
        virtual void onFrame(EventPtr theEvent);
        virtual void realize();
        
        static const char * const SPARK_TYPE;             
        virtual const char * const & getType() const { return Statistic::SPARK_TYPE;};
    private:
        masl::UInt64 lasttime_;      
        TextPtr _myFPSText;                              
    };

    typedef boost::shared_ptr<Statistic> StatisticPtr;
    typedef MemberFunctionEventCallback<Statistic, StatisticPtr> StatisticCB;
        
};
#endif 