// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_masl_Material_h_included_
#define _ac_mobile_masl_Material_h_included_

#include <map>
#include <vector>
#include <string>

#include <masl/Exception.h>
#include <masl/Ptr.h>
#include <masl/MatrixStack.h>

#include "GlHeaders.h"
#include "TextureUnit.h"

namespace mar {
    const std::string DEFAULT_VERTEX_SHADER = "default_vertex.glsl";
    const std::string DEFAULT_TEXTURED_VERTEX_SHADER = "default_textured_vertex.glsl";
    const std::string DEFAULT_COLORED_FRAGMENT_SHADER = "default_colored_fragment.glsl";
    const std::string DEFAULT_TEXTURED_FRAGMENT_SHADER = "default_textured_fragment.glsl";
    const std::string ANDROID_MOVIE_FRAGMENT_SHADER = "android_movie_fragment.glsl";
    const unsigned int MAX_NUM_HANDLES = 23;

    DECLARE_EXCEPTION(ProblemWithHandleException, masl::Exception)

    class Material {
    public:
        virtual ~Material();
        virtual void resetGL();
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void loadShader(const matrix & theMatrix);
        virtual void unloadShader();
        void setAlpha(const float theAlpha) {alpha_ = theAlpha;};
        void setCustomValues(const std::map<std::string, float> & theCustomValues);
        void setCustomHandles(const std::map<std::string, float> & theCustomHandles);

        virtual bool isTransparent() const {return alpha_ != 1.0;};
        virtual std::string getAttributesAsString() const;

    protected:
        Material();
        void initShader();
        virtual void setHandles();
        virtual void bindAttributes();
        GLuint getHandle(const std::string & theName) const;

        float alpha_;
        std::string fragmentShader_;
        std::string vertexShader_;
        GLuint shaderProgram_;
        GLuint mvpHandle_;
        GLuint alphaHandle_;
        std::map<std::string, std::pair<GLuint, float> > customHandlesAndValues_; //maps handle string to pair of handle and value
    private:
        void deleteShader();
    };
    typedef masl::Ptr<Material> MaterialPtr;

    class UnlitColoredMaterial : public Material {
    public:
        UnlitColoredMaterial();
        virtual ~UnlitColoredMaterial();
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void loadShader(const matrix & theMatrix);
        void setDiffuseColor(const vector3 & theColor) {
            diffuse_ = vector4(theColor, alpha_);
        }
        void setDiffuseColor(const vector4 & theColor) {
            diffuse_ = theColor;
        }
        vector4 getDiffuseColor() const {
            return diffuse_;
        }
    
        virtual bool isTransparent() const {return Material::isTransparent() || diffuse_[3] != 1.0;};
    private:
        vector4 diffuse_;
        GLuint colorHandle_;

    private:
        virtual void setHandles();
    };
    typedef masl::Ptr<UnlitColoredMaterial> UnlitColoredMaterialPtr;

    class UnlitTexturedMaterial : public Material {
    public:
        UnlitTexturedMaterial(const std::string & theSrc = "");
        UnlitTexturedMaterial(const TexturePtr theTexture);
        UnlitTexturedMaterial(const TextureUnitPtr theTextureUnit);
        virtual ~UnlitTexturedMaterial();
        virtual void resetGL();
        virtual void setShader(const std::string & theVertexShader = "", const std::string & theFragmentShader = "");
        virtual void loadShader(const matrix & theMatrix);
        TextureUnitPtr getTextureUnit() const {return textureUnit_;}
        void setTextureUnit(const TextureUnitPtr theTextureUnit) { textureUnit_ = theTextureUnit;};
        virtual bool isTransparent() const;
        virtual std::string getAttributesAsString() const;
    private:
        virtual void setHandles();
        virtual void bindAttributes();
        //XXX: support multitexturing with multiple texunits
        TextureUnitPtr textureUnit_;
        GLuint textureMatrixHandle_;

    };
    typedef masl::Ptr<UnlitTexturedMaterial> UnlitTexturedMaterialPtr;
};

#endif
