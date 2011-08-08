#ifndef CPPENTRY
#define CPPENTRY

#ifdef __APPLE__
    //iOS
    #include <string>
#elif __ANDROID__
    //Android
    #include <jni.h>
    #include <string>
#endif

namespace cppcore {
    
    class CppEntry {
        public: 
            CppEntry();
            virtual ~CppEntry();
            virtual bool loadSpark(const std::string & theFilename);
        private:
            void loadXML(const std::string & theFilename);
    };
};

#endif //CPPENTRY

