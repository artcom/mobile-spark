#include "Settings.h"

#include "Logger.h"
#include "Exception.h"
#include "string_functions.h"

#ifdef iOS
    #include <iostream>
#elif ANDROID
    #include <android/log.h>
#endif

namespace masl {
    
    const size_t SEVERITIES = 9;
    const char * SeverityName[] = {"PRINT","TESTRESULT","FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE", "DISABLED", 0};
    const char * const LOG_MODULE_VERBOSITY_ENV = "AC_LOG_MODULE_VERBOSITY";
    const char * const LOG_GLOBAL_VERBOSITY_ENV = "AC_LOG_VERBOSITY";

    Logger::Logger() : _myTopLevelLogTag("Unset"), _myGlobalSeverity(SEV_WARNING) {
    }

    Logger::~Logger() {}


    void Logger::setLoggerTopLevelTag(const std::string & theTagString) {
        _myTopLevelLogTag = theTagString;
    }
    
    void
    Logger::setSeverity() {
        const char * myEnv = getenv(LOG_GLOBAL_VERBOSITY_ENV);
        
        std::string myLogLevelString(myEnv);
        _myGlobalSeverity = getSeverityFromString(masl::trimall(myLogLevelString), SEV_WARNING);

        parseEnvModuleSeverity();
    }

    void
    Logger::setSeverity(const Severity theSeverity) {
        _myGlobalSeverity = theSeverity;
        parseEnvModuleSeverity();
    }

    void
    Logger::parseEnvModuleSeverity() {
        const char * myEnv = getenv(LOG_MODULE_VERBOSITY_ENV);
        AC_INFO << LOG_MODULE_VERBOSITY_ENV << " = " << myEnv;
        if (myEnv && strlen(myEnv) > 0) {
            std::string myLogLevelString(myEnv);
            std::string::size_type myColon;
            while ((myColon = myLogLevelString.find_first_of(":;")) != std::string::npos) {
                setModuleSeverity(myLogLevelString.substr(0, myColon));
                myLogLevelString = myLogLevelString.substr(myColon+1);
            }
            if (myLogLevelString.size()) {
                setModuleSeverity(myLogLevelString);
            }
        }
    }

    /**
    return true if theSeverity is higher (numerically smaller) than the verbosity setting
    a different verbosity can be defined for any id range in any module; if there are different
    verbosity settings for an overlapping id region in the same module, the setting for the
    smallest id-range takes precedence.
    */
    bool
    Logger::ifLog(Severity theSeverity, const char * theModule, int theId) {
        if (!_mySeveritySettings.empty()) {
            Severity mySeverity = _myGlobalSeverity;
            const std::string myModule(file_string(theModule)); // remove everything before the last backslash
            // find all setting for a particular module
            std::multimap<std::string,ModuleSeverity>::const_iterator myLowerBound =
                _mySeveritySettings.lower_bound(myModule);
            if (myLowerBound != _mySeveritySettings.end()) {
                std::multimap<std::string,ModuleSeverity>::const_iterator myUpperBound =
                    _mySeveritySettings.upper_bound(myModule);
    
                // find smallest range containing theId with matching module name
                unsigned int myRange = std::numeric_limits<unsigned int>::max();
                for (std::multimap<std::string,ModuleSeverity>::const_iterator myIter = myLowerBound;
                    myIter != myUpperBound; ++myIter)
                {
                    if (myIter->first == myModule) {
                        int myMinId = myIter->second.myMinId;
                        int myMaxId = myIter->second.myMaxId;
                        if (theId >= myMinId && theId <= myMaxId) {
                            unsigned int myNewRange = myMaxId - myMinId;
                            if (myNewRange < myRange) {
                                mySeverity = myIter->second.mySeverity;
                                myRange = myNewRange;
                            }
                        }
                    }
                }
            }
            return theSeverity <= mySeverity;
        }
        return theSeverity <= _myGlobalSeverity;
    }

    void
    Logger::log(/*Time theTime, */ Severity theSeverity, const char * theModule, int theId, const std::string & theText) {
        std::string myLogTag(_myTopLevelLogTag);
        if (theSeverity == SEV_TESTRESULT) {
            myLogTag += "/TestResult/";
        }
        std::ostringstream myText;
        myText << theText;
        if (theSeverity > SEV_PRINT) {
            myText << " [" << lastFileNamePart(theModule) << " at:" << theId << "]";
        }

        #ifdef iOS
        switch (theSeverity) {
            case SEV_TRACE:
                std::cout << myLogTag << " TRACE: " << myText.str() << "\n";
                break;
            case SEV_DEBUG:
                std::cout << myLogTag << " DEBUG: " << myText.str() << "\n";
                break;
            case SEV_INFO:
                std::cout << myLogTag << " INFO: " << myText.str() << "\n";
                break;
            case SEV_WARNING:
                std::cout << myLogTag << " WARNING: " << myText.str() << "\n";
                break;
            case SEV_PRINT:
                std::cout << myLogTag << " LOG: " << myText.str() << "\n";
                break;
            case SEV_ERROR:
                std::cout << myLogTag << " ERROR: " << myText.str() << "\n";
                break;
            case SEV_TESTRESULT :
                std::cout << myLogTag << " TEST: " << myText.str() << "\n";
                break;
            default:
                throw Exception("Unknown logger severity");
                break;
        }

        #elif ANDROID
        switch (theSeverity) {
            case SEV_TRACE :
                __android_log_print(ANDROID_LOG_VERBOSE, myLogTag.c_str(), myText.str().c_str());//__VA_ARGS__)
                break;
            case SEV_DEBUG :
                __android_log_print(ANDROID_LOG_DEBUG, myLogTag.c_str(), myText.str().c_str());//__VA_ARGS__)
                break;
            case SEV_WARNING :
                __android_log_print(ANDROID_LOG_WARN, myLogTag.c_str(), myText.str().c_str());//__VA_ARGS__)
                break;
            case SEV_INFO :
            case SEV_PRINT :
                __android_log_print(ANDROID_LOG_INFO, myLogTag.c_str(), myText.str().c_str());//__VA_ARGS__)
                break;
            case SEV_ERROR :
                __android_log_print(ANDROID_LOG_ERROR, myLogTag.c_str(), myText.str().c_str());//__VA_ARGS__)
                break;
            case SEV_TESTRESULT :
                __android_log_print(ANDROID_LOG_INFO, myLogTag.c_str(), myText.str().c_str());//__VA_ARGS__)
                break;
            default:
                throw Exception("Unknown logger severity");
                break;
        }
        #endif

    }

    void
    Logger::setModuleSeverity(const std::string & theSeverityString,
                               const std::string & theModule,
                               int theMinId, int theMaxId)
    {
        const std::string & myModule = theModule;
        Severity mySeverity = getSeverityFromString(theSeverityString, SEV_DEBUG);
        AC_INFO << "setting verbosity for module " << myModule << " to " << theSeverityString;
        // TODO: remove item when there is an exact match
        _mySeveritySettings.insert(std::pair<const std::string, ModuleSeverity>(
                                    file_string(myModule.c_str()), ModuleSeverity(mySeverity, theMinId, theMaxId)));
    }
    
    Severity
    Logger::getSeverityFromString(const std::string & theString, Severity theDefault) const {
        if (theString.empty()) return theDefault;
        std::vector<std::string> mySeverities(SeverityName, SeverityName + SEVERITIES);
        for (std::vector<std::string>::size_type i = 0; i < mySeverities.size(); ++i) {
            if (mySeverities[i] == theString) {
                return (Severity) i;
            }
        }
        return theDefault;
    }
    
    void
    Logger::setModuleSeverity(const std::string & theSeverityString) {
    
        AC_INFO << "setModuleSeverity " << theSeverityString;
        std::string mySeverityString = theSeverityString;
        std::vector<std::string> mySubStrings;
    
        std::string::size_type mySlash;
        while ((mySlash = mySeverityString.find('/')) != std::string::npos) {
            std::string myString = mySeverityString.substr(0, mySlash);
            mySubStrings.push_back(myString);
            mySeverityString = mySeverityString.substr(mySlash+1);
        }
        if (mySeverityString.size()) {
            mySubStrings.push_back(mySeverityString);
        }
    
        if (mySubStrings.size() < 2) {
            if (!theSeverityString.empty()) {
                AC_ERROR << "### Insufficient arguments for module Severity '" << theSeverityString << "'";
            }
            return;
        }
    
        std::string myModule = mySubStrings[1];
        int myMinId = 0, myMaxId = std::numeric_limits<int>::max();
        if (mySubStrings.size() > 2 && mySubStrings[2].size()) {
            myMinId = as_int(mySubStrings[2]);
        }
        if (mySubStrings.size() > 3 && mySubStrings[3].size()) {
            myMaxId = as_int(mySubStrings[3]);
        }
        setModuleSeverity(mySubStrings[0], myModule, myMinId, myMaxId);
    }
};
