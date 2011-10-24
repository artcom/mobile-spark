// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_RenderCamera_
#define _included_mobile_spark_RenderCamera_

#include <masl/MatrixStack.h>
#include <masl/numeric_functions.h>

#include "Widget.h"


namespace spark {

    enum ProjectionType {
        PERSPECTIVE,
        ORTHONORMAL,
        AUTO_ORTHONORMAL
    };


    class RenderCamera : public Widget {
    public:
        RenderCamera(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~RenderCamera();
        void activate(float theCanvasWidth, float theCanvasHeight);
        const matrix & getProjectionMatrix();

        static const char * const SPARK_TYPE;
        virtual const char * const & getType() const { return RenderCamera::SPARK_TYPE;};
    private:
        matrix _myProjectionMatrix;
        MatrixStack matrixStack;
        ProjectionType _myProjectionType;
        vector3 _myPerspectiveParams;

        static const char * const PerspectiveStr;
        static const char * const OrtohonormalStr;
        static const char * const AutoOrthonormalStr;
    };

    typedef masl::Ptr<RenderCamera> RenderCameraPtr;
};
#endif
