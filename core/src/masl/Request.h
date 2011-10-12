#ifndef _included_mobile_masl_request_
#define _included_mobile_masl_request_

#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>

#include "Callback.h"
#include "Exception.h"
#include "Ptr.h"

namespace masl {

    typedef std::map<std::string, std::string> CookieJar;

    DECLARE_EXCEPTION(INetException, masl::Exception);

    enum AuthentType { BASIC, DIGEST, ANY };

    class RequestManager;
    class RequestCallback;
    typedef masl::Ptr<RequestCallback> RequestCallbackPtr;

    class Request : public boost::enable_shared_from_this<Request> {
        friend class RequestManager;

        public:
            Request(const std::string & theURL, const std::string & theUserAgent = std::string("acMobileSpark"));
            virtual ~Request();
            CURL * getHandle() const;
            long getResponseCode() const;
            std::string getResponseString() const;
            std::vector<char> getResponseBinary() const;
            std::string getErrorString() const;
            const std::string & getURL() const { return _myURL;};
            void setLowSpeedLimit(unsigned theBytesPerSec);
            void setLowSpeedTimeout(unsigned theSeconds);
            void setTimeoutParams(unsigned theBytesPerSec, unsigned theSeconds);
            void setVerbose(bool theVerboseFlag);
            bool getVerbose() const { return _myVerboseFlag; }
            bool getVerifyPeer() const { return _myVerifyPeerFlag; }
            void setSSLCertificate(const std::string & theCertificateFilename);
            void verifyPeer(bool theFlag);
            void setCookies(const CookieJar & theCookies);
            void setCookie(const std::string & theCookie);
            void setResume(long theResumeOffset);
            void setProxy(const std::string & theProxyServer, bool theTunnelFlag = false);
            void setCredentials(const std::string & theUsername, const std::string & thePassword,
                                AuthentType theAuthentType = ANY);
            void setFollowLocation(bool theFollowFlag);
            void setOnDoneCallback(RequestCallbackPtr theOnDoneCallback) { _myOnDoneCallback = theOnDoneCallback; };

            // request-method type methods
            size_t put(const std::string & thePutData);
            size_t post(const std::string & thePostData);
            void http_delete();
            void get();
            void head();

            const std::multimap<std::string,std::string> & getResponseHeaders() const;
            std::string getResponseHeader(const std::string & theHeader) const;
            std::vector<std::string> getAllResponseHeaders(const std::string & theHeader) const;
            unsigned getLowSpeedLimit(void) const;
            unsigned getLowSpeedTimeout(void) const;

            void addHttpHeader(const std::string & theKey, const std::string & theValue);
            void addHttpHeaderAsDate(const std::string & theKey, const time_t & theValue);

            //asl-TODO: I'm not sure if this function is in the right place...
            static std::string urlEncode(const std::string & theUrl);
            static std::string urlDecode(const std::string & theUrl);

        protected:
            // callback hooks
            virtual void onStart();
            virtual size_t onData(const char * theData, size_t theLength);
            virtual void onError(CURLcode theError, long theHttpStatus);
            virtual bool onProgress(double theDownloadTotal, double theCurrentDownload,
                double theUploadTotal, double theCurrentUpload);  // return false to abort transfer
            virtual void onDone();
            virtual bool onResponseHeader(const std::string & theHeader);

        private:
            // static callback hooks
            static size_t curlWriteCallback( void *theData, size_t theBlockCount,
                                    size_t theBlockSize, void *theRequestObject);
            static int curlProgressCallback( void *theRequestObject, double theDownloadTotal,
                                    double theCurrentDownload, double theUploadTotal,
                                    double theCurrentUpdate);
            static size_t curlHeaderCallback( void *theData, size_t theBlockCount,
                                    size_t theBlockSize, void *theRequestObject);
            //
            Request();
            void checkCurlStatus(CURLcode theStatusCode, const std::string & theWhere) const;

        private:
            std::string         _myURL;
            std::string         _myProxy;
            std::string         _myUserAgent;
            CURL *              _myCurlHandle;
            unsigned            _myLowSpeedLimit;
            unsigned            _myLowSpeedTimeout;
            struct curl_slist * _myHttpHeaderList;
            std::string         _myPostBuffer;
            std::vector<char>   _myResponseBlock;
            std::vector<char>   _myErrorBuffer;
            std::string         _mySSLCertificateFilename;
            std::multimap<std::string, std::string> _myResponseHeaders;
            std::vector<std::string> _myHttpHeaderBuffers;
            std::string         _myCookieBuffer;
            std::string         _myAuthentData;
            bool                _myVerboseFlag;
            bool                _myVerifyPeerFlag;
            RequestCallbackPtr  _myOnDoneCallback;
    };

    typedef masl::Ptr<Request> RequestPtr;

    class SequenceRequest : public Request {
        public:
            SequenceRequest(RequestManager & theRequestManager, const std::string & theURL, 
                            const std::string & theUserAgent = std::string("acMobileSpark"));
            virtual ~SequenceRequest() {};
            void setNextRequest(const RequestPtr theNextRequest) { _myNextRequest = theNextRequest; };
            void setOnAllDoneCallback(RequestCallbackPtr theOnAllDoneCallback) { _myOnAllDoneCallback = theOnAllDoneCallback; };
        protected:
            virtual void onDone();
        private:
            RequestPtr _myNextRequest;
            RequestManager & _myRequestManager;
            RequestCallbackPtr  _myOnAllDoneCallback;
    };
    typedef masl::Ptr<SequenceRequest> SequenceRequestPtr;

    ////////////////////////////////////////////////////////////////////////
    //Request Callbacks ////////////////////////////////////////////////////

    class RequestCallback {
    public:
        virtual ~RequestCallback() {};
        virtual void execute(RequestPtr theRequest) const = 0;
        virtual void operator() (RequestPtr theRequest) {execute(theRequest);};
    };

    typedef masl::Ptr<RequestCallback> RequestCallbackPtr;


    typedef void (*FreeFunctionRequestPtr)(RequestPtr);

    class FreeFunctionRequestCallback : public RequestCallback {
    public:
        FreeFunctionRequestCallback(FreeFunctionRequestPtr theFunctionPtr):
            RequestCallback(),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~FreeFunctionRequestCallback() {};

        virtual void execute(RequestPtr theRequest) const {
            _myFunctionPointer(theRequest);
        };
    private:
        FreeFunctionRequestPtr _myFunctionPointer;
    };
    typedef masl::Ptr<FreeFunctionRequestCallback> FreeFunctionRequestCallbackPtr;



    template < typename T, typename TP>
    class MemberFunctionRequestCallback : public RequestCallback {
    public:
        MemberFunctionRequestCallback(TP theObject, void (T::*theFunctionPtr)(RequestPtr)):
            _myObjectPtr(theObject),
            _myFunctionPointer(theFunctionPtr) {
        };
        virtual ~MemberFunctionRequestCallback() {};

        virtual void execute(RequestPtr theRequest) const {
            (_myObjectPtr.get()->*_myFunctionPointer)(theRequest);
        };
    private:
        TP _myObjectPtr;
        void (T::*_myFunctionPointer)(RequestPtr);
    };


}

#endif
