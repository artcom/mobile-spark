#ifndef _included_UnitTest_
#define _included_UnitTest_

#include "Exception.h"
#include "Logger.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>


class  UnitTest {
public:
    virtual void run() = 0;
    std::string getFailedTestMessages() const;
    unsigned int getPassedCount() const;
    unsigned int getFailedCount() const;
    unsigned int getExpectedFailedCount() const;
    const char * getMyName() const;
    virtual ~UnitTest();
    virtual void incrementPassedCount();
    virtual void incrementFailedCount();
    virtual void incrementExpectedFailedCount();
    int returnStatus() const;
    const char * getTracePrefix ();
    virtual void setSilentSuccess(bool makeSilent = true);
    virtual void setAbortOnFailure(bool makeAbort = true);
    virtual void setup();
    virtual void teardown();
    virtual unsigned getProfileRepeatCount() const;
    virtual void setProfileRepeatCount(unsigned theCount);
private:
    UnitTest();
    UnitTest(const UnitTest &);
    const UnitTest & operator=(const UnitTest &);
protected:
    explicit UnitTest(const char * myName);
public:
    void ensure(bool myExpressionResult,
                const char * myExpression,
                const char * mySourceFileName,
                unsigned long mySourceLine,
                bool myExpectedResult = true);
    template< typename T >
    void ensure(const T& myExpressionResult,
                const char * myExpression,
                const char * mySourceFileName,
                unsigned long mySourceLine,
                bool myExpectedResult = true)
    {ensure(myExpressionResult!=0,myExpression,mySourceFileName,mySourceLine,myExpectedResult);}
protected:
    virtual void setFailedTestMessages(const std::string & theMessages);    
    virtual void setPassedCount(unsigned int passedTests);
    virtual void setFailedCount(unsigned int failedTests);
    virtual void setExpectedFailedCount(unsigned int failedTests);
    virtual void setMyName(const char * theName);
    friend class UnitTestSuite;
private:
    const char * _myName;
    unsigned int _passedCount;
    unsigned int _failedCount;
    unsigned int _expectedfailedCount;
    unsigned int _profilingRepeatCount;
    bool _silentSuccess;
    bool _abortOnFailure;
    std::string _failingTestsMessages;

    void reportResult(bool myExpressionResult,bool myExpectedResult);
};

class UnitTestSuite : public UnitTest {
public:
    explicit UnitTestSuite(const char * myName, int argc, char *argv[])
        : UnitTest(myName), _argc(argc), _argv(argv) {
    }
    ~UnitTestSuite() {
        destroyMyTests();
    }
private:
    UnitTestSuite(UnitTestSuite const& other);
    const UnitTestSuite & operator=(const UnitTestSuite &);

public:
    virtual void addTest(UnitTest * theTest, unsigned int theProfileRepeatCount = 1) {
        if (theTest) {
            _myTests.push_back(theTest);
            theTest->setProfileRepeatCount(theProfileRepeatCount);
            AC_PRINT << ">> Added Test '" << theTest->getMyName() << "', ptr = " << theTest;
        } else {
            AC_PRINT << "## Failed to add Test";
            incrementFailedCount();
        }
    }
    virtual void run();

    virtual void setup() {
        AC_PRINT << ">> Launching Test Suite '" << getMyName() << "'";
    }
    virtual void teardown();
protected:
    int getNumTests() const {
        return _myTests.size();
    }
    UnitTest * getTest(int i) {
        return _myTests[i];
    }
    int get_argc() const {
        return _argc;
    }
    char ** get_argv() const {
        return _argv;
    }
private:
    void destroyMyTests() {
        while (_myTests.size()) {
            delete _myTests.back();
            _myTests.pop_back();
        }
    }
    std::vector<UnitTest *> _myTests;
    int _argc;
    char ** _argv;
};

class TemplateUnitTest : public UnitTest {
public:
    TemplateUnitTest(const char * theClassName, const char * theTemplateArgument)
        : UnitTest(strdup(std::string(std::string(theClassName) + theTemplateArgument).c_str())) {  }
    ~TemplateUnitTest() {
        free((char*)getMyName());
    }
};

template <int N, class TEST>
class Repeat : public TEST {
public:
    Repeat() { }

    template <class ARG>
    Repeat(ARG theArgument) : TEST(theArgument) { }

    void run() {
            AC_ERROR << ">>>>>>>> Repeating Test " << N << " times,  this = " << this << std::endl;
            for (int i = 0; i < N; ++i) {
                TEST::run();
            }
        };
};


#define ENSURE_FULL(EXPRESSION, MESSAGE, EXPECT)    ensure(EXPRESSION, MESSAGE, __FILE__, __LINE__, EXPECT);

#define ENSURE_MSG(EXP, MSG)                        ENSURE_FULL(EXP,MSG ,true );
#define ENSURE(EXP)                                 ENSURE_FULL(EXP,#EXP,true );
#define ENSURE_FAIL_EXPECTED(EXP)                   ENSURE_FULL(EXP,#EXP,false);

#define FAILURE(MSG)                                ENSURE_FULL(false,MSG,true );
#define FAILURE_EXPECTED(MSG)                       ENSURE_FULL(false,MSG,false);
#define SUCCESS(MSG)                                ENSURE_FULL(true ,MSG,true );

#define ENSURE_EXCEPTION(theTest, theException) \
  { \
    try { \
        theTest; \
        FAILURE(#theTest) \
    } catch (const theException &) { \
        SUCCESS(#theTest) \
    } \
 }

#define DTITLE(TITLE)                               AC_ERROR << getTracePrefix() << TITLE << ":" << std::endl;;
#define DPRINT2(TITEL,VARIABLE)                     AC_ERROR << getTracePrefix() << TITEL << ": " << #VARIABLE << " = " << VARIABLE << std::endl;;
#define DPRINT(VARIABLE)                            AC_ERROR << getTracePrefix() << #VARIABLE << " = " << VARIABLE << std::endl;;
#define DDUMP(MULTI_LINE_VARIABLE)                  AC_ERROR << getTracePrefix() << #MULTI_LINE_VARIABLE<< ":" << std::endl << MULTI_LINE_VARIABLE<< std::endl;


#define ENSURE_EQUAL(EXP1,EXP2) { \
    ENSURE_FULL(EXP1 == EXP2, # EXP1 "==" # EXP2, true); \
    if ((EXP1) != (EXP2)) { \
        AC_ERROR << getTracePrefix() << #EXP1 << " = " << EXP1 << std::endl; \
        AC_ERROR << getTracePrefix() << #EXP2 << " = " << EXP2 << std::endl; \
    } \
}

#define ENSURE_ALMOSTEQUAL(EXP1,EXP2) { \
    ENSURE_FULL(almostEqual(EXP1,EXP2), # EXP1 " ~= " # EXP2, true); \
    if (!almostEqual(EXP1, EXP2)) { \
        AC_ERROR << getTracePrefix() << #EXP1 << " = " << EXP1 << std::endl; \
        AC_ERROR << getTracePrefix() << #EXP2 << " = " << EXP2 << std::endl; \
    } \
}

#define ENSURE_UNEQUAL(EXP1,EXP2) { \
    ENSURE_FULL(EXP1 != EXP2, # EXP1 "==" # EXP2, true); \
    if ((EXP1) == (EXP2)) { \
        AC_ERROR << getTracePrefix() << #EXP1 << " = " << EXP1 << std::endl; \
        AC_ERROR << getTracePrefix() << #EXP2 << " = " << EXP2 << std::endl; \
    } \
}

#endif
