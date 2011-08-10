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

#include "UnitTest.h"
#include "Logger.h"

namespace cppcore {
    
    class CppEntry {
        public: 
            CppEntry();
            virtual ~CppEntry();
            virtual std::string test();
            virtual bool loadSpark(const std::string & theFilename);
    };

    /*class CppEntry_UnitTest : public UnitTest {
        public:
            CppEntry_UnitTest() : UnitTest("CppEntry_UnitTest") {  }
            void run() {
                perform_loadSpark();
            }
            void perform_loadSpark() {
                cppcore::CppEntry myCppEntry;
                ENSURE(myCppEntry.loadSpark("/sdcard/test.spark"));
        }
    };*/
                
    class MyTestSuite : public UnitTestSuite {
        public:
            MyTestSuite(const char * myName, int argc, char *argv[]) : UnitTestSuite(myName, argc, argv) {}
            void setup();
    };
        
};

#endif //CPPENTRY

