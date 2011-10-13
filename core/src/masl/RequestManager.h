#ifndef _included_mobile_masl_requestmanager_
#define _included_mobile_masl_requestmanager_

#include "Request.h"

#include <vector>

namespace masl {

    class RequestManager {
        public:
            RequestManager();
            virtual ~RequestManager();
            void performRequest(RequestPtr theRequest);
            // !!!handleRequests must be called periodically
            int handleRequests(bool theBlockingFlag = false);
            int getActiveCount() const { return _myRequests.size(); };
            void setOnErrorCallback(RequestCallbackPtr theOnErrorCallback) { _myDefaultErrorCallback = theOnErrorCallback; };

            //convenience functions
            void getRequest(const std::string & theUrl, const RequestCallbackPtr theCB);
            void postRequest(const std::string & theUrl, const std::string & theData, const RequestCallbackPtr theCB);
            void putRequest(const std::string & theUrl, const std::string & theData, const RequestCallbackPtr theCB);
            void deleteRequest(const std::string & theUrl, const RequestCallbackPtr theCB);
            void getAllRequest(const std::string & theBaseURL, const std::vector<std::string> & theURLLastPartList,
                               const RequestCallbackPtr theOneReadyCB, RequestCallbackPtr theAllReadyCB);


        protected:
            virtual bool removeRequest(Request* theRequest);
        private:
            CURLM * getHandle() { return _myCurlMultiHandle; };
            void checkCurlStatus(CURLMcode theStatusCode,
                    const std::string & theWhere);
            void handleMessages();
            std::vector<RequestPtr> _myRequests;
            CURLM * _myCurlMultiHandle;
            RequestCallbackPtr _myDefaultErrorCallback;
    };
}

#endif

