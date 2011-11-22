// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_masl_requestmanager_
#define _included_mobile_masl_requestmanager_

#include "Request.h"

#include <vector>
#include <utility>

namespace masl {

    enum GetType { REQUEST_ALWAYS, REQUEST_IF_NEWER, REQUEST_IF_NOT_AVAILABLE };
        
    class RequestManager {
        public:
            typedef std::pair<std::string, std::string> HttpHeaderEntry;
            typedef std::vector<HttpHeaderEntry> HttpHeaderEntries;
            typedef  masl::Ptr<HttpHeaderEntries> HttpHeaderEntriesPtr;
                
            RequestManager();
            virtual ~RequestManager();
            void performRequest(RequestPtr theRequest);
            // !!!handleRequests must be called periodically
            int handleRequests(bool theBlockingFlag = false);
            int getActiveCount() const { return _myRequests.size(); };
            void setOnErrorCallback(RequestCallbackPtr theOnErrorCallback) { _myDefaultErrorCallback = theOnErrorCallback; };

            //convenience functions
            void getRequest(const std::string & theUrl, const RequestCallbackPtr theCB,
                            const std::string & thePersistenceFolder = "", 
                            const bool thePersistFlag = false, const GetType theGetType = REQUEST_ALWAYS);
            void headRequest(const std::string & theUrl, const RequestCallbackPtr theCB);
            void postRequest(const std::string & theUrl, const std::string & theData, const RequestCallbackPtr theCB);
            void putRequest(const std::string & theUrl, const std::string & theData, const RequestCallbackPtr theCB, 
                            HttpHeaderEntries & theHttpHeader);
            void putRequest(const std::string & theUrl, const std::string & theData, const RequestCallbackPtr theCB);
            void deleteRequest(const std::string & theUrl, const RequestCallbackPtr theCB);
            void getAllRequest(const std::string & theBaseURL, const std::vector<std::string> & theURLLastPartList,
                               const RequestCallbackPtr theOneReadyCB, 
                               RequestCallbackPtr theAllReadyCB = RequestCallbackPtr(),
                               const std::string & thePersistenceFolder = "",
                               const bool thePersistFlag = false, const GetType theGetType = REQUEST_ALWAYS);
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

