#ifndef LOG_H_
#define LOG_H_


#ifdef __APPLE__
    //iOS
    #include <string>
    #include <sstream>
    #import <CoreFoundation/CoreFoundation.h>

    //TODO find armv7 CFSTRING library
    //extern void NSLog(CFStringRef format, ...); 
    
    #define LOG_TAG location_string(__FILE__,__LINE__).c_str()
    #define LOGI(...) printf("%s, %s \n", LOG_TAG,__VA_ARGS__)
    #define LOGE(...) printf("%s, %s \n", LOG_TAG,__VA_ARGS__)

#elif __ANDROID__
    //Android
    #include <string>
    #include <sstream>
    #include <android/log.h>
    #define LOG_TAG location_string(__FILE__,__LINE__).c_str()
    #define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
    #define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

//////////////////logging



static std::string
lastFileNamePart(const char* file_name) {
    std::string myFilename(file_name);
    std::string::size_type mySlash = myFilename.find_last_of("/\\");
    if (mySlash != std::string::npos) {
        myFilename = myFilename.substr(mySlash+1);
    }
    return myFilename;
}

static std::string
location_string(const char* file_name, unsigned line_number) {
    char buf[20];
    sprintf(buf,"%i",line_number);
    return std::string("[") + lastFileNamePart(file_name) + ":" + std::string(buf) + "]";
}

static float stof(std::string theString) {
    std::stringstream ss(theString);
    float f = 0.0f;
    ss >> f;
    return f;
}

#endif /* LOG_H_ */
