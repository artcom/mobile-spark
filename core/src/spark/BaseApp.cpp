
#include "BaseApp.h"

#include <masl/Logger.h>
#include <masl/XMLUtils.h>


namespace spark {

    BaseApp::BaseApp(): step(0), _myAnimate(true) {
    }

    BaseApp::~BaseApp() {
    }

    void BaseApp::onFrame() {
        //AC_PRINT << "onFrame";
        if (_myAnimate) {
            step += 0.0001f;
            if (step > 1.0f) {
                step = 0.0f;
            }
        }

        window->render();
    }

    void BaseApp::onTouch() {
        AC_PRINT << "onTOUCH";
        _myAnimate = !_myAnimate;
    }
}

