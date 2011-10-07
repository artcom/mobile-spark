#ifndef _included_mobile_acprojectview_AppLoaderAnim_
#define _included_mobile_acprojectview_AppLoaderAnim_

#include <spark/Transform.h>
#include <spark/Event.h>
#include <spark/Image.h>

namespace acprojectview {

    class AppLoaderAnim : public spark::Transform {
        public: 
            AppLoaderAnim(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~AppLoaderAnim();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return AppLoaderAnim::SPARK_TYPE;};
                
            void removeOnFrameListener(spark::EventPtr theEvent);
            void realize();
            void onFrame(spark::EventPtr theEvent);

                    
        private:
            double _myLastTime;
            int _myCurrentImageIndex;
            std::vector<spark::ImagePtr> _myImages;
            spark::EventCallbackPtr _myFrameCB;
            
    };
    typedef masl::Ptr<AppLoaderAnim>AppLoaderAnimPtr;
    typedef spark::MemberFunctionEventCallback<AppLoaderAnim, AppLoaderAnimPtr> AppLoaderAnimCB;
        
};

#endif
