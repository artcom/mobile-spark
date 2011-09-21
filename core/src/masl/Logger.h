#ifndef _ac_mobile_masl_Logger_h_included_
#define _ac_mobile_masl_Logger_h_included_

#include <string>
#include <sstream>

#include "Singleton.h"

namespace masl {
    enum Severity {SEV_PRINT = 0, SEV_TESTRESULT, SEV_FATAL, SEV_ERROR, SEV_WARNING, SEV_INFO, SEV_DEBUG, SEV_TRACE, SEV_DISABLED};

    /**
     * Reads the environment variables 'AC_LOG_VERBOSITY' and 'AC_MODULE_VERBOSITY'
     * to initialize the global logging verbosity and the per-module verbosities.
     *
     * Per-module verbosity has the following format:
     * AC_MODULE_VERBOSITY := MODULE_VERBOSITY | AC_MODULE_VERBOSITY:MODULE_VERBOSITY
     * MODULE_VERBOSITY := <LEVEL>/<MODULE>[/<FROMLINE>[/<TOLINE>]]
     */
    class Logger : public Singleton<Logger> {
        struct ModuleSeverity {
            ModuleSeverity() {}
            ModuleSeverity(Severity theSeverity,int theMinId, int myMaxId)
                : mySeverity(theSeverity), myMinId(theMinId), myMaxId(myMaxId)
            {}

            Severity mySeverity;
            int myMinId;
            int myMaxId;
        };
        public:
            Logger();
            virtual ~Logger();

            /**
            Used to detect if a message should be logged depending on its severity and the logger severity settings.
            returns true if theSeverity is higher (numerically smaller) than the verbosity setting
            a different verbosity can be defined for any id range in any module; if there are different
            verbosity settings for an overlapping id region in the same module, the setting for the
            smallest id-range takes precedence.
            */
            bool ifLog(Severity theSeverity, const char * theModule, int theId);
            void log(/*masl::Time theTime,*/ Severity theSeverity, const char * theModule, int theId, const std::string & theText);
            void setLoggerTopLevelTag(const std::string & theTagString);
            void setSeverity(const Severity theSeverity);
            /**
            sets the verbosity for a particular module; an id range can be supplied;
            the default settings will be that all module ids (line numbers) in a range will be selected.
            */
            void setModuleSeverity(const std::string & theSeverity,
                const std::string & theModule,
                int theMidId = 0,
                int theMaxId = std::numeric_limits<int>::max() );
    
            /** sets the verbosity for a particular module; an id range can be supplied;
                the default settings will be that all module ids (line numbers) in a range will be selected. */
            void setModuleSeverity(const std::string & theSeverity);

        private:
            Severity getSeverityFromString(std::string theString, Severity theDefault) const;
            void parseEnvModuleSeverity();
            std::string _myTopLevelLogTag;
            Severity _myGlobalSeverity;
            std::multimap<std::string,ModuleSeverity> _mySeveritySettings;
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
