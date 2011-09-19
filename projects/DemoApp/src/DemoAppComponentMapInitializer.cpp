#include "DemoAppComponentMapInitializer.h"
#include <spark/SparkComponentFactory.h>
#include "Slide.h"

namespace demoapp {
    void DemoAppComponentMapInitializer::init() {
        spark::SparkComponentFactory::get().registerComponent(SlideImpl::SPARK_TYPE, spark::create<SlideImpl>);
    }
}
