// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "ObjImporter.h"

#include <boost/tuple/tuple_comparison.hpp>
#include <masl/AssetProvider.h>
#include <masl/Logger.h>
#include <masl/string_functions.h>

#define DB(x) // x

using namespace masl;

namespace mar {
    ObjImporter::~ObjImporter() {}

    ////////////////////////////////Assumptions
    //obj is exported with normals_ and texcoords
    //obj can consist of multiple elements with diverse materials
    //geometry is triangulated
    //filename is given with extension
    //material file .mtl has the same name as .obj file
    //color of textured materials is ignored

    void ObjImporter::faceParseHelper(std::vector<int> &theFaceData, const std::string & theData) const {
        size_t pos1 = theData.find_first_of("/");
        size_t pos2 = theData.find_last_of("/");
        std::string a = theData.substr(0,pos1);
        std::string b = theData.substr(pos1+1, pos2-pos1-1);
        std::string c = theData.substr(pos2+1);
        theFaceData.push_back(a != "" ? as<int>(a) : 0);
        theFaceData.push_back(b != "" ? as<int>(b) : 0);
        theFaceData.push_back(c != "" ? as<int>(c) : 0);
    }

    vector3 ObjImporter::getVector3(const std::string & theString) const {
        vector3 myVector;
        size_t posAB = theString.find_first_of(" ");
        size_t posBC = theString.find_last_of(" ");
        std::string a = theString.substr(0,posAB);
        std::string b = theString.substr(posAB+1,posBC-posAB-1);
        std::string c = theString.substr(posBC+1);
        myVector[0] = as<float>(a);
        myVector[1] = as<float>(b);
        myVector[2] = as<float>(c);
        return myVector;
    }

    vector4 ObjImporter::getColor(const std::string & theString) const {
        vector4 myVector;
        vector3 myV = getVector3(theString);
        myVector[0] = myV[0];
        myVector[1] = myV[1];
        myVector[2] = myV[2];
        myVector[3] = 1;
        return myVector;
    }

    void ObjImporter::checkBB(const vector3 & theVertex) {
        for (size_t i = 0; i < 3; ++i) {
            if (theVertex[i] < min_[i]) {
                min_[i] = theVertex[i];
            }
            if (theVertex[i] > max_[i]) {
                max_[i] = theVertex[i];
            }
        }
    }

    void ObjImporter::importMaterialMap(const std::vector<std::string> & theMtlFile) {
        MaterialPtr myMaterial;
        std::string myMaterialId;
        for (std::vector<std::string>::const_iterator it = theMtlFile.begin(); it != theMtlFile.end(); ++it) {
            std::string line = *it;
            DB(AC_DEBUG << "line " << line.c_str();)
            size_t pos = line.find_first_of(" ");
            if (pos == std::string::npos) {
                continue;
            }
            std::string type = line.substr(0,pos);
            std::string data = line.substr(pos+1);
            if (type == "newmtl") {
                if (myMaterial) {  //save old material in map
                    materialMap_[myMaterialId] = myMaterial;
                }
                myMaterialId = data;
                myMaterial = MaterialPtr(new UnlitColoredMaterial()); //here we do not know if we need textures, use default material, exchange later if necessary
            } else if (type == "Ka") {
                //TODO: not supported yet
                //vector4 myColor = getColor(data);
                //boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->ambient_ = myColor;
            } else if (type == "Kd") {
                vector4 myColor = getColor(data);
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->setDiffuseColor(myColor);
            } else if (type == "Ks") {
                //TODO: not supported yet
                //vector4 myColor = getColor(data);
                //boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->specular_ = myColor;
            } else if (type == "d" || type == "Tr") {
                UnlitColoredMaterialPtr myUnlitColoredMaterial = boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial);
                vector4 myDiffuseColor = myUnlitColoredMaterial->getDiffuseColor();
                myDiffuseColor[3] = masl::as<float>(data);
                myUnlitColoredMaterial->setDiffuseColor(myDiffuseColor);
            } else if (type == "Ns") {
                //not supported yet
                //boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->shininess_ = masl::as<float>(data);
            } else if (type == "illum") {
                //not supported yet
                //boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->illuminationModel_ = masl::as<int>(data);
            } else if (type == "map_Kd") {
                //here we know that we need textures -> exchange default material with textured material                
                myMaterial = MaterialPtr(new UnlitTexturedMaterial(trimall("textures/" + data)));
            }
        }
        if (myMaterial) {
            materialMap_[myMaterialId] = myMaterial;
        }
        AC_DEBUG << "num materials " << materialMap_.size();
    }

    void ObjImporter::createElementVertices(ShapePtr theShape, ElementPtr element,
                                         size_t startFaceIndex) {
        size_t dataPerVertex = 3 + 3 + 2;
        element->numIndices_ = (faces_.size() - startFaceIndex) * 3;
        IndexData myIndexData = IndexData(new IndexData::element_type[element->numIndices_]);
        std::vector<float> temporaryVertexData;
        std::map<boost::tuple<int, int, int>, int> indexMap;
        int myIndex = 0;
        int indexDataPosition = 0;
        std::vector<std::vector<int> >::iterator it = faces_.begin();
        std::advance(it,startFaceIndex);
        for (; it != faces_.end(); ++it) {
            for (size_t i = 0; i < 3; i++) {

                boost::tuple<int, int, int> key = boost::make_tuple((*it)[i * 3 + 0], (*it)[i * 3 + 1], (*it)[i * 3 + 2]);
                if (indexMap.find(key) != indexMap.end()) {
                    myIndexData[indexDataPosition] = indexMap.at(key);
                    indexDataPosition++;
                }
                else {
                    indexMap[key] = myIndex;
                    myIndexData[indexDataPosition] = myIndex;
                    myIndex++;
                    indexDataPosition++;

                    temporaryVertexData.push_back(vertices_[(*it)[i * 3 + 0]-1][0]);
                    temporaryVertexData.push_back(vertices_[(*it)[i * 3 + 0]-1][1]);
                    temporaryVertexData.push_back(vertices_[(*it)[i * 3 + 0]-1][2]);
                    temporaryVertexData.push_back(normals_[(*it)[i * 3 + 2]-1][0]);
                    temporaryVertexData.push_back(normals_[(*it)[i * 3 + 2]-1][1]);
                    temporaryVertexData.push_back(normals_[(*it)[i * 3 + 2]-1][2]);

                    int texId = (*it)[i * 3 + 1]-1;
                    temporaryVertexData.push_back((texId == -1 ? 0 : texData_[texId][0]));
                    temporaryVertexData.push_back((texId == -1 ? 0 : texData_[texId][1]));
                    checkBB(vertices_[(*it)[i * 3 + 0]-1]);
                }
            }
        }
        element->setIndexData(myIndexData);
        element->numVertices_ = myIndex;
        element->setVertexData(VertexData(new VertexData::element_type[element->numVertices_ * dataPerVertex]));
        int vertexDataIndex = 0;
        for (std::vector<float>::const_iterator it = temporaryVertexData.begin(); it != temporaryVertexData.end(); ++it) {
            element->getVertexData()[vertexDataIndex++] = *it;
        }
        theShape->elementList_.push_back(element);
        AC_DEBUG << "num parts " << theShape->elementList_.size();
    }

    bool ObjImporter::sortByTransparencyFunction(ElementPtr i,ElementPtr j) {
        if (!i->material_->isTransparent() && j->material_->isTransparent()) {
            return true;
        } else {
            return false;
        }
    }




    void ObjImporter::importObj(const std::string & theObjFileName, ShapePtr theShape) {
        vertices_.clear();
        normals_.clear();
        texData_.clear();
        faces_.clear();
        materialMap_.clear();

        min_[0] = min_[1] = min_[2] = std::numeric_limits<float>::max();
        max_[0] = max_[1] = max_[2] = std::numeric_limits<float>::min();
        min_[3] = max_[3] = 1;
        AC_DEBUG << "import obj " << theObjFileName;
        const std::vector<std::string> theObjFile =
            masl::AssetProviderSingleton::get().ap()->getLineByLineFromFile(theObjFileName + std::string(".obj"));
        const std::vector<std::string> theMtlFile =
            masl::AssetProviderSingleton::get().ap()->getLineByLineFromFile(theObjFileName + std::string(".mtl"));
        AC_DEBUG << "got data from files " << theObjFile.size() << "  " << theMtlFile.size();
        importMaterialMap(theMtlFile);

        ElementPtr element;

        size_t startFaceIndex = 0;
        for (std::vector<std::string>::const_iterator it = theObjFile.begin(); it != theObjFile.end(); ++it) {
            std::string line = *it;
            DB(AC_DEBUG << "line " << line;)
            size_t pos = line.find_first_of(" ");
            if (pos == std::string::npos) {
                continue;
            }
            std::string type = line.substr(0,pos);
            std::string data = line.substr(pos+1);
            DB(AC_DEBUG << "type " << type << ", data " << data;)
            if (type == "v") {
                vertices_.push_back(getVector3(data));
            } else if (type == "vt") {
                vector2 texCoordinates;
                size_t pos = data.find_first_of(" ");
                std::string s = data.substr(0,pos);
                std::string t = data.substr(pos+1);
                texCoordinates[0] = as<float>(s);
                texCoordinates[1] = as<float>(t);
                texData_.push_back(texCoordinates);
            } else if (type == "vn") {
                normals_.push_back(getVector3(data));
            } else if (type == "usemtl") {
                if (element) {
                    createElementVertices(theShape, element, startFaceIndex);
                }
                element = ElementPtr(new ElementWithNormalsAndTexture());
                element->material_ = materialMap_[data];
                startFaceIndex = faces_.size();
            } else if (type == "f") {
                DB(AC_DEBUG << "data:_" << data;)
                std::vector<int> faceVertices;
                size_t pos1 = data.find_first_of(" ");
                size_t pos2 = data.find_last_of(" ");
                std::string v1 = data.substr(0,pos1);
                std::string v2 = data.substr(pos1+1,pos2-pos1-1);
                std::string v3 = data.substr(pos2+1);

                faceParseHelper(faceVertices,v1);
                faceParseHelper(faceVertices,v2);
                faceParseHelper(faceVertices,v3);
                faces_.push_back(faceVertices);
            }
        }
        if (element) {
            createElementVertices(theShape, element, startFaceIndex);
        }

        std::sort(theShape->elementList_.begin(), theShape->elementList_.end(), sortByTransparencyFunction);
        AC_DEBUG << "vertex size " << vertices_.size() << " normals_ size " << normals_.size() << " tex size " << texData_.size();
        AC_DEBUG << "faces_ size " << faces_.size() << " objectParts " << theShape->elementList_.size();
        theShape->setBoundingBox(min_, max_);
    }
}
