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
            // !!!must be called periodically
            int handleRequests(bool theBlockingFlag = false);
            int getActiveCount() const { return _myRequests.size(); };

            //convenience functions
            void getRequest(const std::string & theUrl, RequestCallbackPtr theCB);

        protected:
            virtual bool removeRequest(Request* theRequest);
        private:
            CURLM * getHandle() { return _myCurlMultiHandle; };
            void checkCurlStatus(CURLMcode theStatusCode,
                    const std::string & theWhere);
            void handleMessages();
            std::vector<RequestPtr> _myRequests;
            CURLM * _myCurlMultiHandle;
    };
}

#endif

