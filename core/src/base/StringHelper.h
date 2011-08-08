#ifndef STRINGHELPER_H_
#define STRINGHELPER_H_

#include <string>
#include <sstream>

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

#endif
