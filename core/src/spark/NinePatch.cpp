#include "NinePatch.h"

#include <mar/TextureLoader.h>
#include <mar/Shape.h>
#include <mar/Material.h>
#include "BaseApp.h"
#include "SparkComponentFactory.h"

using namespace mar;

namespace spark {
    const char * const NinePatch::SPARK_TYPE = "NinePatch";

    NinePatch::NinePatch(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode):
        Image(theApp, theXMLNode),
        edgeLeft_(_myXMLNode->getAttributeAs<float>("edgeLeft", 0)),
        edgeTop_(_myXMLNode->getAttributeAs<float>("edgeTop", 0)),
        edgeRight_(_myXMLNode->getAttributeAs<float>("edgeRight", 0)),
        edgeBottom_(_myXMLNode->getAttributeAs<float>("edgeBottom", 0))
    {}

    NinePatch::~NinePatch() {
    }

    ShapePtr
    NinePatch::createCustomShape(const MaterialPtr theMaterial) {
        NinePatchShapePtr myShape = NinePatchShapePtr(new NinePatchShape(theMaterial));
        myShape->setEdges(edgeLeft_, edgeTop_, edgeRight_, edgeBottom_);
        return myShape;
    }

}
