#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_


#include <boost/smart_ptr/shared_ptr.hpp>
#include <mar/AssetProvider.h>
#include <spark/Window.h>


namespace spark {
    class BaseApp {
        public: 
            BaseApp();
            virtual ~BaseApp();

            virtual bool setup(std::string assetPath, std::string layoutFile);
            virtual void onFrame();
            virtual void onTouch();

            spark::WindowPtr window; 
            float step;
            mar::AssetProviderPtr assetProvider;
        protected:
            bool _myAnimate;
    };

    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
};

#endif
