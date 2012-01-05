// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_masl_AssetProvider_h_included_
#define _ac_mobile_masl_AssetProvider_h_included_

#include <vector>
#include "Singleton.h"
#include "Logger.h"
#include "Ptr.h"


namespace masl {
    class AssetProvider {
    public:
        virtual std::string getStringFromFile(const std::string & theFile) const = 0;
        virtual std::vector<char> getBlockFromFile(const std::string & theFileName) const = 0;
        virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const = 0;
        virtual bool loadTextureFromPNG(const std::string & filename, unsigned int & textureId, int & width, int & height, bool & rgb) = 0;
        virtual bool loadTextureFromFile(const std::string & filename, unsigned int & textureId, unsigned int & width, unsigned int & height, bool & hasAlpha) = 0;
        virtual void addIncludePath(const std::string & thePath) = 0;
        virtual void storeInFile(const std::string & theFileName, const std::string & theData) = 0;
        virtual void storeInFile(const std::string & theFileName, const std::vector<char> & theData) = 0;

        virtual std::vector<std::string> getFilesFromPath(const std::string & thePath, const std::string & thePattern = "", const bool theForceOnlyInBundle_APK = false) const;
        virtual std::string findFile(const std::string & theFilename) const;
        virtual std::string getAssetPath() const { return assetPath_; };
        virtual std::string getDownloadsPath() const = 0;
        virtual std::string getDownloadsFolder() const = 0;

        const std::vector<std::string> & getIncludePaths() { return includePaths_;}
    protected:
        std::vector<std::string> includePaths_;
        std::string assetPath_;
    };
    typedef masl::Ptr<AssetProvider> AssetProviderPtr;


    class AssetProviderSingleton : public masl::Singleton<AssetProviderSingleton> {
        public:
            virtual ~AssetProviderSingleton();
            void setAssetProvider(AssetProviderPtr theAssetProvider);
            const AssetProviderPtr & ap() const { return _myAssetProvider;};

        private:
            AssetProviderPtr _myAssetProvider;
    };

};

#endif
