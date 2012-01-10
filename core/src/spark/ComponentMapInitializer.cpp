// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "ComponentMapInitializer.h"
#include "SparkComponentFactory.h"

#include "Camera.h"
#include "HttpTransform.h"
#include "Image.h"
#include "Movie.h"
#include "I18nContext.h"
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
        SparkComponentFactory::get().registerComponent(HttpTransform::SPARK_TYPE, spark::create<HttpTransform>);
        SparkComponentFactory::get().registerComponent(Image::SPARK_TYPE, spark::create<Image>);
        SparkComponentFactory::get().registerComponent(Movie::SPARK_TYPE, spark::create<Movie>);
        SparkComponentFactory::get().registerComponent(I18nContext::SPARK_TYPE, spark::create<I18nContext>);
        SparkComponentFactory::get().registerComponent(I18nItem::SPARK_TYPE, spark::create<I18nItem>);
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
