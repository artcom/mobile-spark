#include "DemoAppComponentMapInitializer.h"
#include <spark/SparkComponentFactory.h>
#include "Slide.h"

namespace demoapp {
    void DemoAppComponentMapInitializer::init() {
        spark::SparkComponentFactory::get().registerComponent("SlideImpl", spark::create<SlideImpl>);
    }
}
