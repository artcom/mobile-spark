#include "AppProvider.h"

namespace spark {
    AppProvider::~AppProvider() {
    }

    void
    AppProvider::setApp(BaseAppPtr theApp) {
        app_ = theApp;
    }
}
