#include "ObjImporter.h"

#include <masl/Logger.h>
#include <masl/string_functions.h>

#include "AssetProvider.h"
#include "png_functions.h"

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

    void ObjImporter::faceParseHelper(std::vector<int> &theFaceData, const std::string & theData) {
        size_t pos1 = theData.find_first_of("/");
        size_t pos2 = theData.find_last_of("/");
        std::string a = theData.substr(0,pos1);
        std::string b = theData.substr(pos1+1, pos2-pos1-1);
        std::string c = theData.substr(pos2+1);
        theFaceData.push_back(a != "" ? as<int>(a) : 0);
        theFaceData.push_back(b != "" ? as<int>(b) : 0);
        theFaceData.push_back(c != "" ? as<int>(c) : 0);
    }

    vector3 ObjImporter::getVector3(const std::string & theString) {
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

    vector4 ObjImporter::getColor(const std::string & theString) {
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
            //AC_PRINT << "line " << line.c_str();
            size_t pos = line.find_first_of(" ");
            if (pos == std::string::npos) {
                continue;
            }
            std::string type = line.substr(0,pos);
            std::string data = line.substr(pos+1);
            if (type == "newmtl") {
                if (myMaterial) {  //save old material in map
                    materialMap_[myMaterialId] = myMaterial;
                    myMaterial->createShader();
                }
                myMaterialId = data;
                myMaterial = MaterialPtr(new UnlitColoredMaterial()); //XXX: here we do not know if we need textures
            } else if (type == "Ka") {
                vector4 myColor = getColor(data);
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->ambient = myColor;
            } else if (type == "Kd") {
                vector4 myColor = getColor(data);
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->setDiffuseColor(myColor);
            } else if (type == "Ks") {
                vector4 myColor = getColor(data);
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->specular = myColor;
            } else if (type == "d" || type == "Tr") {
                UnlitColoredMaterialPtr myUnlitColoredMaterial = boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial);
                vector4 myDiffuseColor = myUnlitColoredMaterial->getDiffuseColor();
                myDiffuseColor[3] = masl::as<float>(data);
                myUnlitColoredMaterial->setDiffuseColor(myDiffuseColor);
            } else if (type == "Ns") {
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->shininess = masl::as<float>(data);
            } else if (type == "illum") {
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->illuminationModel = masl::as<int>(data);
            } else if (type == "map_Kd") {
                //XXX: here we know that we need textures
                myMaterial = MaterialPtr(new UnlitTexturedMaterial("assets/models/textures/" + data));
                //loadTextureFromPNG("assets/models/textures/" + data, boost::static_pointer_cast<UnlitTexturedMaterial>(myMaterial));
                //needed?
                //glBindTexture(GL_TEXTURE_2D, myTextureID);
                ////non-clamp wrap-modes do only work with power-of-2-textures
                //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }        
        }
        if (myMaterial) {
            materialMap_[myMaterialId] = myMaterial;
            myMaterial->createShader();
        }
        AC_PRINT << "num materials " << materialMap_.size();
    }

    void ObjImporter::createElementVertices(ShapePtr theShape, ElementPtr element,
                                         size_t startFaceIndex) {
        size_t dataPerVertex = 3 + 3 + 2;
        size_t stride = dataPerVertex * 3;
        element->numVertices = (faces_.size() - startFaceIndex) * 3;
        element->vertexData_ = boost::shared_array<float>(new float[(element->numVertices) * dataPerVertex]);
        int myIndex = 0;
        std::vector<std::vector<int> >::iterator it = faces_.begin();
        advance(it,startFaceIndex);
        for (; it != faces_.end(); ++it) {
            //AC_PRINT << "indices " << (*it)[0]-1 << "  " << (*it)[1]-1 << "  " << (*it)[2]-1;
            element->vertexData_[myIndex * stride + 0] = vertices_[(*it)[0]-1][0];
            element->vertexData_[myIndex * stride + 1] = vertices_[(*it)[0]-1][1];
            element->vertexData_[myIndex * stride + 2] = vertices_[(*it)[0]-1][2];
            element->vertexData_[myIndex * stride + 3] = normals_[(*it)[2]-1][0];
            element->vertexData_[myIndex * stride + 4] = normals_[(*it)[2]-1][1];
            element->vertexData_[myIndex * stride + 5] = normals_[(*it)[2]-1][2];
            int texId = (*it)[1]-1;
            element->vertexData_[myIndex * stride + 6] = (texId == -1 ? 0 : texData_[texId][0]);
            element->vertexData_[myIndex * stride + 7] = (texId == -1 ? 0 : texData_[texId][1]);
            checkBB(vertices_[(*it)[0]-1]);

            //AC_PRINT << "indices " << (*it)[3]-1 << "  " << (*it)[4]-1 << "  " << (*it)[5]-1;
            element->vertexData_[myIndex * stride + 8] = vertices_[(*it)[3]-1][0];
            element->vertexData_[myIndex * stride + 9] = vertices_[(*it)[3]-1][1];
            element->vertexData_[myIndex * stride + 10] = vertices_[(*it)[3]-1][2];
            element->vertexData_[myIndex * stride + 11] = normals_[(*it)[5]-1][0];
            element->vertexData_[myIndex * stride + 12] = normals_[(*it)[5]-1][1];
            element->vertexData_[myIndex * stride + 13] = normals_[(*it)[5]-1][2];
            texId = (*it)[4]-1;
            element->vertexData_[myIndex * stride + 14] = (texId == -1 ? 0 : texData_[(*it)[4]-1][0]);
            element->vertexData_[myIndex * stride + 15] = (texId == -1 ? 0 : texData_[(*it)[4]-1][1]);
            checkBB(vertices_[(*it)[3]-1]);

            //AC_PRINT << "indices " << (*it)[6]-1 << "  " << (*it)[7]-1 << "  " << (*it)[8]-1;
            element->vertexData_[myIndex * stride + 16] = vertices_[(*it)[6]-1][0];
            element->vertexData_[myIndex * stride + 17] = vertices_[(*it)[6]-1][1];
            element->vertexData_[myIndex * stride + 18] = vertices_[(*it)[6]-1][2];
            element->vertexData_[myIndex * stride + 19] = normals_[(*it)[8]-1][0];
            element->vertexData_[myIndex * stride + 20] = normals_[(*it)[8]-1][1];
            element->vertexData_[myIndex * stride + 21] = normals_[(*it)[8]-1][2];
            texId = (*it)[7]-1;
            element->vertexData_[myIndex * stride + 22] = (texId == -1 ? 0 : texData_[(*it)[7]-1][0]);
            element->vertexData_[myIndex * stride + 23] = (texId == -1 ? 0 : texData_[(*it)[7]-1][1]);
            checkBB(vertices_[(*it)[6]-1]);
            myIndex++;
        }
        theShape->elementList.push_back(element);
        //AC_PRINT << "num parts " << theShape->elementList.size();
    }

    bool ObjImporter::sortByTransparencyFunction(ElementPtr i,ElementPtr j) { 
        if (!i->material->transparency_ && j->material->transparency_) {
            return true;
        } else {
            return false;
        }
    }




    void ObjImporter::importObj(std::string theObjFileName, ShapePtr theShape) {
        vertices_.clear();
        normals_.clear();
        texData_.clear();
        faces_.clear();
        materialMap_.clear();
        min_[0] = min_[1] = min_[2] = std::numeric_limits<float>::max();
        max_[0] = max_[1] = max_[2] = std::numeric_limits<float>::min();
        min_[3] = max_[3] = 1;
        //AC_PRINT << "import obj " << theObjFileName;
        const std::vector<std::string> theObjFile = 
            AssetProviderSingleton::get().ap()->getLineByLineFromFile(theObjFileName + std::string(".obj"));
        const std::vector<std::string> theMtlFile = 
            AssetProviderSingleton::get().ap()->getLineByLineFromFile(theObjFileName + std::string(".mtl"));
        //AC_PRINT << "got data from files " << theObjFile.size() << "  " << theMtlFile.size();
        importMaterialMap(theMtlFile);

        ElementPtr element;

        size_t startFaceIndex = 0;
        for (std::vector<std::string>::const_iterator it = theObjFile.begin(); it != theObjFile.end(); ++it) {
            std::string line = *it;
            //AC_PRINT << "line " << line;
            size_t pos = line.find_first_of(" ");
            if (pos == std::string::npos) {
                continue;
            }
            std::string type = line.substr(0,pos);
            std::string data = line.substr(pos+1);
            //AC_PRINT << "type " << type << ", data " << data;
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
                element->material = materialMap_[data];
                startFaceIndex = faces_.size();
            } else if (type == "f") {
                //AC_PRINT << "data:_" << data;
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

        std::sort(theShape->elementList.begin(), theShape->elementList.end(), sortByTransparencyFunction);
        AC_PRINT << "vertex size " << vertices_.size() << " normals_ size " << normals_.size() << " tex size " << texData_.size();
        AC_PRINT << "faces_ size " << faces_.size() << " objectParts " << theShape->elementList.size();
        theShape->setBoundingBox(min_, max_);
    }
}
