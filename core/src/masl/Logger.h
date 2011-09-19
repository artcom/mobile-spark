#ifndef _ac_mobile_masl_Logger_h_included_
#define _ac_mobile_masl_Logger_h_included_

#include <string>
#include <sstream>

#include "Singleton.h"

namespace masl {
    enum Severity {SEV_PRINT = 0, SEV_TESTRESULT, SEV_FATAL, SEV_ERROR, SEV_WARNING, SEV_INFO, SEV_DEBUG, SEV_TRACE, SEV_DISABLED};
    
    class Logger : public Singleton<Logger> {
        public:
            Logger();
            virtual ~Logger();
                      
            inline bool ifLog(/*masl::Time theTime,*/ Severity theSeverity, const char * theModule, int theId) {
                return theSeverity <= _myGlobalSeverity;
            }
            void log(/*masl::Time theTime,*/ Severity theSeverity, const char * theModule, int theId, const std::string & theText);     
            void setLoggerTopLevelTag(const std::string & theTagString);
            void setSeverity(const Severity theSeverity) { _myGlobalSeverity = theSeverity;};
        private:
            std::string _myTopLevelLogTag;
            Severity _myGlobalSeverity;
    };
    
    /**
    This class is used to collect the output and deliver it to the Logger on destruction
    */    
    class MessagePort {
    public:
        MessagePort(Severity theSeverity, const char * theModule, int theId)
            : mySeverity(theSeverity), myModule(theModule), myId(theId)
        {}
        ~MessagePort() {            
            Logger::get().log(/*myTime,*/ mySeverity, myModule, myId, stream.str());
        }
        /* This getter is used in the gcc branch to avoid printing of the first message token
         * as a hex address. I still do not understand what exactly goes wrong when accessing
         * the member directly. However, this solves bug #24.
         */
        inline std::ostringstream & getStream() {
            return stream;
        }
        std::ostringstream stream;
        //const Time myTime;
        const Severity mySeverity;
        const char * myModule;
        const int myId;
    };

#define AC_LOG_CHECK(SEVERITY,MODULE,MSGID) masl::Logger::get().ifLog(SEVERITY,MODULE,MSGID) && (masl::MessagePort(SEVERITY,MODULE,MSGID).getStream())

#define AC_INFO AC_LOG_CHECK(masl::SEV_INFO, __FILE__ ,__LINE__)
#define AC_TRACE AC_LOG_CHECK(masl::SEV_TRACE, __FILE__ ,__LINE__)
#define AC_DEBUG AC_LOG_CHECK(masl::SEV_DEBUG, __FILE__ ,__LINE__)
#define AC_PRINT AC_LOG_CHECK(masl::SEV_PRINT, __FILE__ ,__LINE__)
#define AC_ERROR AC_LOG_CHECK(masl::SEV_ERROR, __FILE__ ,__LINE__)
#define AC_WARNING AC_LOG_CHECK(masl::SEV_WARNING, __FILE__ ,__LINE__)
#define AC_TEST_RESULT AC_LOG_CHECK(masl::SEV_TESTRESULT, __FILE__ ,__LINE__)

};
#endif
