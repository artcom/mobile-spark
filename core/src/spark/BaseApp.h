#ifndef _included_mobile_spark_BaseApp_
#define _included_mobile_spark_BaseApp_


#include <boost/smart_ptr/shared_ptr.hpp>
#include <mar/AssetProvider.h>
#include <mar/Canvas.h>

#include "Window.h"



namespace spark {

    class BaseApp {
        public: 
            BaseApp();
            virtual ~BaseApp();

            virtual bool setup(const long theCurrentMillis, const std::string & theAssetPath, const std::string & theLayoutFile);
            virtual void onFrame(const long theCurrentMillis);
            virtual void onSizeChanged(int theWidth, int theHeight);
            virtual void onTouch();

            spark::WindowPtr _mySparkWindow; 
            mar::AssetProviderPtr assetProvider;
        protected:
            bool _myAnimate;
        private:
            mar::CanvasPtr _myGLCanvas;            
    };

    typedef boost::shared_ptr<BaseApp> BaseAppPtr;
};

#endif
