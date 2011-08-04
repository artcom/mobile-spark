#ifndef CPPENTRY
#define CPPENTRY

#include <jni.h>
#include <string>

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

