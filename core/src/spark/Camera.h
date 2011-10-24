// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_Camera_
#define _included_mobile_spark_Camera_

#include "ShapeWidget.h"

namespace spark {

    class Camera : public ShapeWidget {
    public:
        Camera(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Camera();
        virtual void prerender(MatrixStack& theCurrentMatrixStack);
        virtual void onPause();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return Camera::SPARK_TYPE;};
    private:
        void setGeometry();
        bool _myColorConversionFlag;
    };

    typedef masl::Ptr<Camera> CameraPtr;
    typedef MemberFunctionEventCallback<Camera, CameraPtr> CameraCB;

};
#endif
