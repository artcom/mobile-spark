// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "View.h"

#include <string>
#include <boost/progress.hpp>

#include <masl/XMLNode.h>
#include <mar/openGL_functions.h>
#include <mar/Viewport.h>

#include "SparkComponentFactory.h"
#include "Widget.h"
#include "Visitors.h"

using namespace masl;
using namespace mar;
using namespace std;

#define DBT(x) //x

namespace spark {

    const char * const View::SPARK_TYPE = "View";

    View::View(const BaseAppPtr theApp, const XMLNodePtr theXMLNode):
        Widget(theApp, theXMLNode),
        _myWorldName(theXMLNode->getAttributeAs<std::string>("world", ""))
    {
        vector2 myPos = theXMLNode->getAttributeAs<vector2>("pos", vector2(0,0));
        vector2 mySize = theXMLNode->getAttributeAs<vector2>("size", vector2(1,1));
        _myGLViewport = ViewportPtr(new Viewport(mySize[0],mySize[1], myPos[0],myPos[1]));
    }

    View::~View() {
    }

    void
    View::ensureCamera() {
        if (_myCamera) {
            return;
        }
        string myCameraName = _myXMLNode->getAttributeAs<std::string>("cameraName", "");
        // find camera
        ContainerPtr myRoot = boost::static_pointer_cast<Container>(getRoot());
        _myCamera = boost::static_pointer_cast<spark::RenderCamera>(myRoot->getChildByName(myCameraName, true));
    }

    void
    View::renderWorld(ComponentPtr theWorld) {
        ensureCamera();
        AC_TRACE << " View::renderWorld ------------------ ";
        WidgetPtr myWorld = boost::static_pointer_cast<spark::Widget>(theWorld);
        myWorld->prerender(matrixStack);
        mar::BoundingBox myVisibleBB;
        myVisibleBB.min[0] = _myGLViewport->getPos()[0];
        myVisibleBB.min[1] = _myGLViewport->getPos()[1];
        myVisibleBB.max[0] = (_myGLViewport->getPos()[0] + _myGLViewport->getSize()[0]);
        myVisibleBB.max[1] = (_myGLViewport->getPos()[1] + _myGLViewport->getSize()[1]);        
        DBT(boost::timer::timer myTimer;)
        RenderList myRenderList;
        CollectVisibleNodesVisitor myVisitor(myRenderList, myVisibleBB, _myCamera->getProjectionMatrix());
        parentFirstVisitComponents(myVisitor, myWorld);
        stable_sort(myRenderList.begin(), myRenderList.end(), sortByRenderKey);
        DBT(AC_PRINT << "               View::renderWord create renderlist: " << myTimer.elapsed() << " s";)

        DBT(boost::timer::timer myTimer2;)
        unsigned myRenderCount = 0;            
        for (RenderList::const_iterator it = myRenderList.begin(); it != myRenderList.end(); ++it) {
            AC_TRACE << " View::renderWorld render component: " << it->first->getName();
            it->first->render(_myCamera->getProjectionMatrix());
            myRenderCount++;
        }
        DBT(AC_PRINT << "               View::renderWord render renderlist: " << myTimer2.elapsed() << " s" << " rendered objects : #" << myRenderCount;)
    }

    void
    View::activate(float theCanvasWidth, float theCanvasHeight) {
        ensureCamera();
        matrixStack.clear();
        vector2 mySize = _myGLViewport->getSize();
        _myCamera->activate(mySize[0] * theCanvasWidth, mySize[1] * theCanvasHeight);
        _myGLViewport->activate(theCanvasWidth, theCanvasHeight);
        _myCanvasWidth = theCanvasWidth;
        _myCanvasHeight = theCanvasHeight;
    }

    std::string 
    View::getAttributesAsString() const {
        return Widget::getAttributesAsString() + " world=\""+_myWorldName+"\" pos=\""+
            masl::as_string(_myGLViewport->getPos())+"\" size=\""+masl::as_string(_myGLViewport->getSize())+"\""
            " camera=\""+_myCamera->getName()+"\"";
    }

}
