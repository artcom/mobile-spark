#include "ObjImporter.h"

#include <masl/Logger.h>

#include "AssetProvider.h"
#include "png_functions.h"

namespace mar {

    ////////////////////////////////Assumptions
    //obj is exported with normals and texcoords
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
        theFaceData.push_back(atoi(a.c_str()));
        theFaceData.push_back(atoi(b.c_str()));
        theFaceData.push_back(atoi(c.c_str()));
    }

    std::vector<float> ObjImporter::getFloatTriple(const std::string & theString) {
        std::vector<float> triple;
        size_t posAB = theString.find_first_of(" ");
        size_t posBC = theString.find_last_of(" ");
        std::string a = theString.substr(0,posAB); 
        std::string b = theString.substr(posAB+1,posBC-posAB-1); 
        std::string c = theString.substr(posBC+1); 
        triple.push_back(atof(a.c_str()));
        triple.push_back(atof(b.c_str()));
        triple.push_back(atof(c.c_str()));
        return triple;
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
                    materialMap[myMaterialId] = myMaterial;
                    myMaterial->createShader();
                }
                myMaterialId = data;
                myMaterial = MaterialPtr(new UnlitColoredMaterial()); //XXX: here we do not know if we need textures
            } else if (type == "Ka") {
                std::vector<float> myColor = getFloatTriple(data);
                myColor.push_back(1);
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->ambient = myColor;
            } else if (type == "Kd") {
                std::vector<float> myColor = getFloatTriple(data);
                myColor.push_back(1);
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->diffuse = myColor;
            } else if (type == "Ks") {
                std::vector<float> myColor = getFloatTriple(data);
                myColor.push_back(1);
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->specular = myColor;
            } else if (type == "d") {
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->transparency = atof(data.c_str());
            } else if (type == "Tr") {
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->transparency = atof(data.c_str());
            } else if (type == "Ns") {
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->shininess = atoi(data.c_str());
            } else if (type == "illum") {
                boost::static_pointer_cast<UnlitColoredMaterial>(myMaterial)->illuminationModel = atof(data.c_str());
            } else if (type == "map_Kd") {
                //XXX: here we know that we need textures
                myMaterial = MaterialPtr(new UnlitTexturedMaterial());
                loadTextureFromPNG("assets/models/textures/" + data, boost::static_pointer_cast<UnlitTexturedMaterial>(myMaterial));
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
            materialMap[myMaterialId] = myMaterial;
            myMaterial->createShader();
        }
        AC_PRINT << "num materials " << materialMap.size();
    }

    void ObjImporter::createElementVertices(ShapePtr theShape, ElementPtr element,
                                         size_t startFaceIndex) {
        size_t dataPerVertex = 3 + 3 + 2;
        size_t stride = dataPerVertex * 3;
        element->numVertices = (faces.size() - startFaceIndex) * 3;
        element->vertexData = boost::shared_array<float>(new float[(element->numVertices) * dataPerVertex]);
        int myIndex = 0;
        std::vector<std::vector<int> >::iterator it = faces.begin();
        advance(it,startFaceIndex);
        for (; it != faces.end(); ++it) {
            //AC_PRINT << "indices " << (*it)[0]-1 << "  " << (*it)[1]-1 << "  " << (*it)[2]-1;
            element->vertexData[myIndex * stride + 0] = vertices[(*it)[0]-1][0];
            element->vertexData[myIndex * stride + 1] = vertices[(*it)[0]-1][1];
            element->vertexData[myIndex * stride + 2] = vertices[(*it)[0]-1][2];
            element->vertexData[myIndex * stride + 3] = normals[(*it)[2]-1][0];
            element->vertexData[myIndex * stride + 4] = normals[(*it)[2]-1][1];
            element->vertexData[myIndex * stride + 5] = normals[(*it)[2]-1][2];
            int texId = (*it)[1]-1;
            element->vertexData[myIndex * stride + 6] = (texId == -1 ? 0 : texData[texId][0]);
            element->vertexData[myIndex * stride + 7] = (texId == -1 ? 0 : texData[texId][1]);

            //AC_PRINT << "indices " << (*it)[3]-1 << "  " << (*it)[4]-1 << "  " << (*it)[5]-1;
            element->vertexData[myIndex * stride + 8] = vertices[(*it)[3]-1][0];
            element->vertexData[myIndex * stride + 9] = vertices[(*it)[3]-1][1];
            element->vertexData[myIndex * stride + 10] = vertices[(*it)[3]-1][2];
            element->vertexData[myIndex * stride + 11] = normals[(*it)[5]-1][0];
            element->vertexData[myIndex * stride + 12] = normals[(*it)[5]-1][1];
            element->vertexData[myIndex * stride + 13] = normals[(*it)[5]-1][2];
            texId = (*it)[4]-1;
            element->vertexData[myIndex * stride + 14] = (texId == -1 ? 0 : texData[(*it)[4]-1][0]);
            element->vertexData[myIndex * stride + 15] = (texId == -1 ? 0 : texData[(*it)[4]-1][1]);

            //AC_PRINT << "indices " << (*it)[6]-1 << "  " << (*it)[7]-1 << "  " << (*it)[8]-1;
            element->vertexData[myIndex * stride + 16] = vertices[(*it)[6]-1][0];
            element->vertexData[myIndex * stride + 17] = vertices[(*it)[6]-1][1];
            element->vertexData[myIndex * stride + 18] = vertices[(*it)[6]-1][2];
            element->vertexData[myIndex * stride + 19] = normals[(*it)[8]-1][0];
            element->vertexData[myIndex * stride + 20] = normals[(*it)[8]-1][1];
            element->vertexData[myIndex * stride + 21] = normals[(*it)[8]-1][2];
            texId = (*it)[7]-1;
            element->vertexData[myIndex * stride + 22] = (texId == -1 ? 0 : texData[(*it)[7]-1][0]);
            element->vertexData[myIndex * stride + 23] = (texId == -1 ? 0 : texData[(*it)[7]-1][1]);
            myIndex++;
        }
        theShape->elementList.push_back(element);
        //AC_PRINT << "num parts " << theShape->elementList.size();
    }

    bool ObjImporter::sortByTransparencyFunction(ElementPtr i,ElementPtr j) { 
        if (!i->material->transparency && j->material->transparency) {
            return true;
        } else {
            return false;
        }
    }




    void ObjImporter::importObj(std::string theObjFileName, ShapePtr theShape) {
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
                vertices.push_back(getFloatTriple(data));
            } else if (type == "vt") {
                std::vector<float> texCoordinates;
                size_t pos = data.find_first_of(" ");
                std::string s = data.substr(0,pos);
                std::string t = data.substr(pos+1);
                texCoordinates.push_back(atof(s.c_str()));
                texCoordinates.push_back(atof(t.c_str()));
                texData.push_back(texCoordinates); 
            } else if (type == "vn") {
                normals.push_back(getFloatTriple(data));
            } else if (type == "usemtl") {
                if (element) {
                    createElementVertices(theShape, element, startFaceIndex);
                }
                element = ElementPtr(new ElementWithNormalsAndTexture());
                element->material = materialMap[data];
                startFaceIndex = faces.size();
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
                faces.push_back(faceVertices);
            }
        }
        if (element) {
            createElementVertices(theShape, element, startFaceIndex);
        }

        std::sort(theShape->elementList.begin(), theShape->elementList.end(), sortByTransparencyFunction);
        AC_PRINT << "vertex size " << vertices.size() << " normals size " << normals.size() << " tex size " << texData.size();
        AC_PRINT << "faces size " << faces.size() << " objectParts " << theShape->elementList.size();
    }
}
