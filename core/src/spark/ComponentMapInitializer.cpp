#include "ComponentMapInitializer.h"
#include "SparkComponentFactory.h"
#include "Camera.h"
#include "Image.h"
#include "NinePatch.h"
#include "Rectangle.h"
#include "RenderCamera.h"
#include "Shape3D.h"
#include "Statistic.h"
#include "Text.h"
#include "Transform.h"
#include "View.h"
#include "Window.h"
#include "World.h"

namespace spark {
    void ComponentMapInitializer::init() {
        SparkComponentFactory::get().registerComponent(Camera::SPARK_TYPE, spark::create<Camera>);
        SparkComponentFactory::get().registerComponent(Image::SPARK_TYPE, spark::create<Image>);
        SparkComponentFactory::get().registerComponent(NinePatch::SPARK_TYPE, spark::create<NinePatch>);
        SparkComponentFactory::get().registerComponent(Rectangle::SPARK_TYPE, spark::create<Rectangle>);
        SparkComponentFactory::get().registerComponent(RenderCamera::SPARK_TYPE, spark::create<RenderCamera>);
        SparkComponentFactory::get().registerComponent(Shape3D::SPARK_TYPE, spark::create<Shape3D>);
        SparkComponentFactory::get().registerComponent(Statistic::SPARK_TYPE, spark::create<Statistic>);
        SparkComponentFactory::get().registerComponent(Text::SPARK_TYPE, spark::create<Text>);
        SparkComponentFactory::get().registerComponent(Transform::SPARK_TYPE, spark::create<Transform>);
        SparkComponentFactory::get().registerComponent(View::SPARK_TYPE, spark::create<View>);
        SparkComponentFactory::get().registerComponent(Window::SPARK_TYPE, spark::create<Window>);
        SparkComponentFactory::get().registerComponent(World::SPARK_TYPE, spark::create<World>);
    }
}
