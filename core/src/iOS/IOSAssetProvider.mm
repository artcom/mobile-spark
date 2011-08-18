#import "IOSAssetProvider.h"

IOSAssetProvider::IOSAssetProvider(const std::string & theAssetFolderPath)
: AssetProvider(), _myAssetFolderPath(theAssetFolderPath)
{
}

IOSAssetProvider::~IOSAssetProvider()
{
}

std::string IOSAssetProvider::getStringFromFile(const std::string & theFile) const 
{
    std::string content = "";
    NSScanner *scanner = [NSScanner scannerWithString:[NSString stringWithUTF8String:theFile.c_str()]];
    NSString *resourcePath;
    NSString *resourceType;
    [scanner scanUpToString:@"." intoString:&resourcePath];
    [scanner scanString:@"." intoString:NULL];
    resourceType = [[scanner string] substringToIndex:[scanner scanLocation]];
    NSString *sparkFilePath = [[NSBundle mainBundle] pathForResource:resourcePath ofType:resourceType inDirectory:[NSString stringWithUTF8String:_myAssetFolderPath.c_str()]];
    NSLog(@"%@", sparkFilePath);
    masl::readFile([sparkFilePath UTF8String], content);
    return content;
}