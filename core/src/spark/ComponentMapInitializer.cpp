#include "ComponentMapInitializer.h"
#include "SparkComponentFactory.h"
#include "Camera.h"
#include "Image.h"
#include "NinePatch.h"
#include "Rectangle.h"
#include "RenderCamera.h"
#include "Shape3D.h"
#include "Text.h"
#include "Transform.h"
#include "View.h"
#include "Window.h"
#include "World.h"

namespace spark {
    void ComponentMapInitializer::init() {
        SparkComponentFactory::get().registerComponent("Camera", spark::create<Camera>);
        SparkComponentFactory::get().registerComponent("Image", spark::create<Image>);
        SparkComponentFactory::get().registerComponent("NinePatch", spark::create<NinePatch>);
        SparkComponentFactory::get().registerComponent("Rectangle", spark::create<Rectangle>);
        SparkComponentFactory::get().registerComponent("RenderCamera", spark::create<RenderCamera>);
        SparkComponentFactory::get().registerComponent("Shape3D", spark::create<Shape3D>);
        SparkComponentFactory::get().registerComponent("Text", spark::create<Text>);
        SparkComponentFactory::get().registerComponent("Transform", spark::create<Transform>);
        SparkComponentFactory::get().registerComponent("View", spark::create<View>);
        SparkComponentFactory::get().registerComponent("Window", spark::create<Window>);
        SparkComponentFactory::get().registerComponent("World", spark::create<World>);
    }
}
