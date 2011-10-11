#ifndef _included_mobile_masl_request_
#define _included_mobile_masl_request_

#ifdef USE_URL

#include <string>
#include <curl/curl.h>
#include <vector>
#include <map>

#include "Exception.h"
#include "Ptr.h"

namespace masl {

    typedef std::map<std::string, std::string> CookieJar;

    DEFINE_EXCEPTION(INetException, masl::Exception);

    enum AuthentTypeEnum {
        BASIC,
        DIGEST,
        ANY,
        AuthentTypeEnum_MAX
    };

    const char * const AuthentTypeStrings[] = {
        "BASIC",
        "DIGEST",
        "ANY",
        ""
    };

    DEFINE_ENUM(AuthentType, AuthentTypeEnum, Y60_INET_DECL);

    class RequestManager;

    /**
     * @ingroup y60inet
     * HTTP Request.
     *
     */
    class Y60_INET_DECL Request {
        friend class RequestManager;

        public:
            Request(const std::string & theURL, const std::string & theUserAgent = std::string("Y60/")+asl::ourRevision);
            virtual ~Request();
            CURL * getHandle() const;
            long getResponseCode() const;
            std::string getResponseString() const;
            const asl::Block & getResponseBlock() const;
            std::string getErrorString() const;
            const std::string & getURL() const;
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

            // request-method type methods
            size_t put(const std::string & thePutData);
            size_t putBlock(const asl::Ptr<asl::Block> & theBlock);
            size_t post(const std::string & thePostData);
            size_t postFile(const std::string & theFilename);
            size_t postBlock(const asl::Ptr<asl::ReadableBlock> & theBlock);
            void http_delete();
            void get();
            void head();

            const std::multimap<std::string,std::string> & getResponseHeaders() const;
            std::string getResponseHeader(const std::string & theHeader) const;
            std::vector<std::string> getAllResponseHeaders(const std::string & theHeader) const;
            time_t getResponseHeaderAsDate(const std::string & theHeader) const;
            unsigned getLowSpeedLimit(void) const;
            unsigned getLowSpeedTimeout(void) const;

            void addHttpHeader(const std::string & theKey, const std::string & theValue);
            void addHttpHeaderAsDate(const std::string & theKey, const time_t & theValue);

            //TODO: I'm not sure if this function is in the right place...
            static std::string urlEncode(const std::string & theUrl);
            static std::string urlDecode(const std::string & theUrl);

            static time_t getTimeFromHTTPDate(const std::string & theHTTPDate );

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
            asl::Ptr<asl::ReadableBlock> _myPostBlock;
            asl::Block          _myResponseBlock;
            std::vector<char>   _myErrorBuffer;
            std::string         _mySSLCertificateFilename;
            std::multimap<std::string, std::string> _myResponseHeaders;
            std::vector<std::string> _myHttpHeaderBuffers;
            std::string         _myCookieBuffer;
            std::string         _myAuthentData;
            bool                _myVerboseFlag;
            bool                _myVerifyPeerFlag;
    };

    typedef asl::Ptr<Request> RequestPtr;
    typedef asl::WeakPtr<Request> RequestWeakPtr;

}

#endif
#endif
