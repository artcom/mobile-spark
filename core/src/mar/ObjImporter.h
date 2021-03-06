// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_mar_ObjImporter_h_included_
#define _ac_mobile_mar_ObjImporter_h_included_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <masl/Singleton.h>
#include <masl/Exception.h>
#include <masl/numeric_functions.h>
#include "Shape.h"
#include "Material.h"

namespace mar {

    class ObjImporter : public masl::Singleton<ObjImporter> {
    public:
        virtual ~ObjImporter();

        void importObj(const std::string & theObjFileName, ShapePtr theShape);
    private:
        void faceParseHelper(std::vector<int> &theFaceData, const std::string & theData) const;
        vector3 getVector3(const std::string & theString) const;
        vector4 getColor(const std::string & theString) const;
        void importMaterialMap(const std::vector<std::string> & theMtlFile);
        void createElementVertices(ShapePtr theShape, ElementPtr element,
                                         size_t startFaceIndex);
        void checkBB(const vector3 & theVertex);
        static bool sortByTransparencyFunction(ElementPtr i,ElementPtr j);

        std::vector<vector3 > vertices_;
        std::vector<vector3 > normals_;
        std::vector<vector2> texData_;
        std::vector<std::vector<int> > faces_;
        std::map<std::string, MaterialPtr> materialMap_;
        vector4 min_;
        vector4 max_;
    };

};

#endif
