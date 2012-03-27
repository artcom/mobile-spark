// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_ios_AssetProvider_
#define _included_mobile_ios_AssetProvider_

#include <masl/Ptr.h>
#include <masl/AssetProvider.h>

namespace ios {
class IOSAssetProvider : public masl::AssetProvider {
public:
    IOSAssetProvider(const std::string & theAssetFolderPath, const std::string & theAppPath);
    virtual ~IOSAssetProvider();
    virtual std::string getStringFromFile(const std::string & theFile) const;
    virtual std::vector<char> getBlockFromFile(const std::string & theFileName) const;
    virtual std::vector<std::string> getLineByLineFromFile(const std::string & theFile) const;
    virtual bool loadTextureFromFile(const std::string & filename, unsigned int & textureId, 
                                     unsigned int & width, unsigned int & height, 
                                     unsigned int & realwidth, unsigned int & realheight, 
                                     matrix & npotMatrix,
                                     bool & hasAlpha, bool & theMirrorFlag, bool & theMipmapFlag);                    
    virtual void addIncludePath(const std::string & thePath)  { includePaths_.push_back(assetPath_ + "/" + thePath); };
    virtual void storeInFile(const std::string & theFileName, const std::string & theData);
    virtual void storeInFile(const std::string & theFileName, const std::vector<char> & theData);
    
    virtual std::string getDownloadsPath() const;
    virtual std::string getDownloadsFolder() const;

private:
    const std::string _myAssetFolderPath;
};
typedef masl::Ptr<IOSAssetProvider> IOSAssetProviderPtr;
};

#endif
