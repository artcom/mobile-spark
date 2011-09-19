// own header
#include "UnitTest.h"
#include "string_functions.h"

using namespace std;
using namespace masl;


unsigned int
UnitTest::getPassedCount() const {
    return _passedCount;
}
string
UnitTest::getFailedTestMessages() const {
    return _failingTestsMessages;
}

unsigned int
UnitTest::getFailedCount() const {
    return _failedCount;
}

unsigned int
UnitTest::getExpectedFailedCount() const {
    return _expectedfailedCount;
}

const char *
UnitTest::getMyName() const {
    return _myName;
}


UnitTest::~UnitTest() {
}

void
UnitTest::incrementPassedCount() {
    ++_passedCount;
}

void
UnitTest::incrementFailedCount() {
    ++_failedCount;
}

void
UnitTest::incrementExpectedFailedCount() {
    ++_expectedfailedCount;
}

int
UnitTest::returnStatus() const {
    if (getPassedCount() != 0 && getFailedCount() == 0) {
        return 0;
    } else {
        return 1;
    }
}

void
UnitTest::setSilentSuccess(bool makeSilent) {
    _silentSuccess = makeSilent;
}

void
UnitTest::setAbortOnFailure(bool makeAbort) {
    _abortOnFailure = makeAbort;
}

void
UnitTest::setup() {
    AC_TEST_RESULT << ">>>> Launching Test Unit '" << _myName << "'";
}

void
UnitTest::teardown() {
    string myTearDownString;
    myTearDownString = ">>>> Completed Test Unit '" + string(getMyName()) +  "', ";

    if (getFailedCount()) {
//        std::cerr << TTYRED;
    }
    myTearDownString += as_string(getFailedCount()) + " total tests failed unexpected";
    //AC_TEST_RESULT << getFailedCount() << " total tests failed unexpected";// << ENDCOLOR;

    myTearDownString += ", ";
    if (getExpectedFailedCount()) {
//        std::cerr << TTYYELLOW;
    }
    myTearDownString += as_string(getExpectedFailedCount()) + " total tests failed expected";
    
    //AC_TEST_RESULT << getExpectedFailedCount() << " total tests failed expected";// << ENDCOLOR;

    myTearDownString += ", ";
    
    if (getPassedCount()) {
//        std::cerr << TTYGREEN;
    }
    myTearDownString += as_string(getPassedCount()) + " total tests passed";

    //AC_TEST_RESULT << getPassedCount() << " total tests passed";// << ENDCOLOR;

    AC_TEST_RESULT << myTearDownString;

}

UnitTest::UnitTest(const char * myName)
    : _myName(myName)
    , _passedCount(0), _failedCount(0), _expectedfailedCount(0)
    , _profilingRepeatCount(0)
    , _silentSuccess(false), _abortOnFailure(false) { }

void
UnitTest::ensure(bool myExpressionResult,
        const char * myExpression,
        const char * mySourceFileName,
        unsigned long mySourceLine,
        bool myExpectedResult)
{
    string myEnsureResultString;
    reportResult(myExpressionResult,myExpectedResult);
    if (myExpressionResult == myExpectedResult) {
        if (_silentSuccess) {
            return;
        }
        myEnsureResultString += ">>>>>> ";
        myEnsureResultString += (myExpectedResult ? "OK    " : "KNOWN ");
        //AC_TEST_RESULT << ">>>>>> " /*<< (myExpectedResult ? TTYGREEN : TTYYELLOW)*/
        //          << (myExpectedResult ? "OK    " : "KNOWN ");// << ENDCOLOR;
    } else {
        _failingTestsMessages += "###### ";
        _failingTestsMessages += (myExpectedResult ? "FAIL    " : "UNEXP ");
        _failingTestsMessages += " (" +  string(myExpression) + ") [" + lastFileNamePart(mySourceFileName) + " at:" + as_string(mySourceLine) + "]";
        _failingTestsMessages += "\n";
        myEnsureResultString += "###### ";
        myEnsureResultString += (myExpectedResult ? "FAIL    " : "UNEXP ");
        //AC_TEST_RESULT << "###### " /*<< (myExpectedResult ? TTYRED : TTYYELLOW)*/
        //          << (myExpectedResult ? "FAIL  " : "UNEXP " );// << ENDCOLOR;
    }
    myEnsureResultString += " (" +  string(myExpression) + ") [" + lastFileNamePart(mySourceFileName) + " at:" + as_string(mySourceLine) + "]";
    //AC_TEST_RESULT << " ("<< myExpression << "), Line " << as_string(mySourceLine);
    //std::cerr << " ("<< myExpression << "), Line " << mySourceLine << std::endl;
    AC_TEST_RESULT << myEnsureResultString;
    if (!myExpressionResult && _abortOnFailure) {
        AC_TEST_RESULT << "UnitTest::ensure: Execution aborted" << std::endl;
        abort();
    }
}

const char *
UnitTest::getTracePrefix () {
    return ">>>>>>        ";
}

void 
UnitTest::setFailedTestMessages(const string & theMessages) {
    _failingTestsMessages = theMessages;
}  
void
UnitTest::setPassedCount(unsigned int passedTests) {
    _passedCount = passedTests;
}
void
UnitTest::setFailedCount(unsigned int failedTests) {
    _failedCount = failedTests;
}
void
UnitTest::setExpectedFailedCount(unsigned int failedTests) {
    _expectedfailedCount = failedTests;
}

void
UnitTest::setProfileRepeatCount(unsigned int theCount) {
    _profilingRepeatCount = theCount;
}

unsigned int
UnitTest::getProfileRepeatCount() const {
    return _profilingRepeatCount;
}

void
UnitTest::setMyName(const char * theName) {
    _myName = theName;
}


void
UnitTest::reportResult(bool myExpressionResult, bool myExpectedResult)
{
    if( myExpressionResult ) {
        if( myExpectedResult ) {
            incrementPassedCount(); // expected success
        } else {
            incrementPassedCount(); // unexpected success
        }
    } else {
        if( myExpectedResult ) {
            incrementFailedCount(); // unexpected failure
        } else {
            incrementExpectedFailedCount(); // expected failure
        }
    }
}

void
UnitTestSuite::run() {
    try {
        try {
            masl::Exception::initExceptionBehaviour();
#ifndef _WIN32
//            masl::initSignalHandling();
#endif
            setup();
        } catch (std::exception & e) {
            std::cerr /*<< TTYRED*/ << "## A std::exception occured during setup of test suite '"
                << getMyName() /*<< ENDCOLOR*/ << "':" << std::endl << e.what() << std::endl;
            incrementFailedCount();
            throw;
        } catch (masl::Exception & e) {
            std::cerr /*<< TTYRED*/ << "## An exception occured during setup of test suite '"
                << getMyName() /*<< ENDCOLOR*/ << "':" << std::endl << e << std::endl;
            incrementFailedCount();
            throw;
        } catch (...) {
            std::cerr /*<< TTYRED*/ << "## An exception occured during setup of test suite '"
                << getMyName() << "'" /*<< ENDCOLOR*/ << std::endl;
            incrementFailedCount();
            throw;
        }
        try {
            for (unsigned i = 0; i < _myTests.size(); ++i) {
                _myTests[i]->setup();
                _myTests[i]->run();
                setPassedCount(getPassedCount() + _myTests[i]->getPassedCount());
                setFailedCount(getFailedCount() + _myTests[i]->getFailedCount());
                setExpectedFailedCount(getExpectedFailedCount() + _myTests[i]->getExpectedFailedCount());
                if (_myTests[i]->getFailedTestMessages().length() > 0 ) {
                    setFailedTestMessages(getFailedTestMessages() + "\n" + _myTests[i]->getMyName() + "\n" + _myTests[i]->getFailedTestMessages());
                }
                _myTests[i]->teardown();
            }
        } catch (std::exception & e) {
            std::cerr /*<< TTYRED*/ << "## A std::exception occured during execution of test suite '"
                << getMyName() /*<< ENDCOLOR */<< "':" << std::endl << e.what() << std::endl;
            incrementFailedCount();
            throw;
        } catch (masl::Exception & e) {
            std::cerr /*<< TTYRED */ << "## An exception occured during execution of test suite '"
                << getMyName() << "':" /*<< ENDCOLOR */<< std::endl << e << std::endl;
            incrementFailedCount();
            throw;
        } catch (...) {
            std::cerr /*<< TTYRED*/ << "## An unknown exception occured during execution of test suite '"
                << getMyName() << "'" /*<< ENDCOLOR*/ << std::endl;
            incrementFailedCount();
            throw;
        }
        try {
            teardown();
        } catch (std::exception & e) {
            std::cerr /*<< TTYRED*/ << "## A std::exception occured during teardown of test suite '"
                << getMyName() << /*ENDCOLOR << */"':" << std::endl << e.what() << std::endl;
            incrementFailedCount();
            throw;
        } catch (masl::Exception & e) {
            std::cerr << "## An exception occured during teardown of test suite '"
                << getMyName() << "':" << std::endl << e << std::endl;
            incrementFailedCount();
            throw;
        } catch (...) {
            std::cerr << "## An unknown exception occured during teardown of test suite '"
                << getMyName() << "'" << std::endl;
            incrementFailedCount();
            throw;
        }
    } catch (...) {
        std::cerr << "## test suite '" << getMyName() << "'"
            << " was not completed because of an unknown exception" << std::endl;
        throw;
    }
}


void
UnitTestSuite::teardown() {
    destroyMyTests();

    UnitTest::teardown();
    std::cerr << std::endl;
}
