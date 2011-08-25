#ifndef _ac_mobile_mar_ObjImporter_h_included_
#define _ac_mobile_mar_ObjImporter_h_included_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <masl/Singleton.h>
#include "Shape.h"
#include "Material.h"

namespace mar {
    class ObjImporter : public masl::Singleton<ObjImporter> {
    public:
        virtual ~ObjImporter();
        
        void importObj(std::string theObjFileName, ShapePtr theShape);
    private:
        std::vector<std::vector<float> > vertices;
        std::vector<std::vector<float> > normals;
        std::vector<std::vector<float> > texData;
        std::vector<std::vector<int> > faces;
        std::map<std::string, MaterialPtr> materialMap;

        void faceParseHelper(std::vector<int> &theFaceData, const std::string & theData);
        std::vector<float> getFloatTriple(const std::string & theString); 
        void importMaterialMap(const std::vector<std::string> & theMtlFile); 
        void createElementVertices(ShapePtr theShape, ElementPtr element,
                                         size_t startFaceIndex);
        static bool sortByTransparencyFunction(ElementPtr i,ElementPtr j); 
    };

};

#endif