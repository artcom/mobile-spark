#include "RequestManager.h"

#include <iostream>
#include <curl/curl.h>

#include "Logger.h"

#define DB(x) // x;


namespace masl {
    void
    RequestManager::checkCurlStatus(CURLMcode theStatusCode, const std::string & theWhere) {
        if (theStatusCode != CURLM_OK && theStatusCode != CURLM_CALL_MULTI_PERFORM) {
            throw INetException(std::string("Curl Multi Error: ") + as_string(theStatusCode), theWhere);
        }
    }

    RequestManager::RequestManager() {
        curl_global_init(CURL_GLOBAL_ALL);
        _myCurlMultiHandle = curl_multi_init();
    }

    RequestManager::~RequestManager() {
        while (!_myRequests.empty()) {
            RequestPtr myRequest = _myRequests.back();
            curl_multi_remove_handle(_myCurlMultiHandle, myRequest->getHandle());
            _myRequests.pop_back();
        }

        CURLMcode myStatus = curl_multi_cleanup(_myCurlMultiHandle);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
    }

    void
    RequestManager::performRequest(RequestPtr theRequest) {
        theRequest->onStart();
        curl_multi_add_handle(_myCurlMultiHandle, theRequest->getHandle());
        _myRequests.push_back(theRequest);

    }

    bool 
    RequestManager::removeRequest(Request* theRequest) {
        std::vector<RequestPtr>::iterator it;

        for (it=_myRequests.begin();  it != _myRequests.end(); ++it) {
            if (&(*(*it)) == theRequest) {
                curl_multi_remove_handle(_myCurlMultiHandle, theRequest->getHandle());
                _myRequests.erase(it);
                return true;
            }
        }  
        return false;
    }

    void
    RequestManager::handleMessages() {
        int myMessageCount = 0;
        CURLMsg * myMessage = 0;
        do {
            myMessage = curl_multi_info_read(_myCurlMultiHandle, &myMessageCount);
            if (myMessage) {
                CURL * myEasyHandle = myMessage->easy_handle;
                Request * myRequest = 0;
                curl_easy_getinfo(myEasyHandle, CURLINFO_PRIVATE, &myRequest);
                if (myMessage->msg == CURLMSG_DONE) {
                    CURLcode myResult = myMessage->data.result;
                    if (myResult == CURLE_OK) {
                        long myResponseCode = myRequest->getResponseCode();
                        if (myResponseCode / 100 == 2) {
                            myRequest->onDone();
                        } else {
                            myRequest->onError(myResult, myResponseCode);
                        }
                    } else {
                        AC_ERROR << "curl request failed with error: " << curl_easy_strerror(myResult);
                        myRequest->onError(myResult, 0);
                    }
                    removeRequest(myRequest);
                } else {
                    throw INetException("RequestManager::handleMessages(): unknown curl msg", PLUS_FILE_LINE);
                }
            }
        } while (myMessage);


    }

    int
    RequestManager::handleRequests(bool theBlockingFlag) {
        CURLMcode myStatus;

        AC_TRACE << "handleRequests("<<theBlockingFlag<<");";

        if (theBlockingFlag) {
            fd_set myReadHandles;
            fd_set myWriteHandles;
            fd_set myExceptHandles;
            int myMaxFd;

            struct timeval tv;
            /* Wait up to 0.1 sec */
            tv.tv_sec = 0;
            tv.tv_usec = 100000;

            FD_ZERO(&myReadHandles);
            FD_ZERO(&myWriteHandles);
            FD_ZERO(&myExceptHandles);
            myStatus = curl_multi_fdset(_myCurlMultiHandle,
                    &myReadHandles, &myWriteHandles,&myExceptHandles,
                    &myMaxFd);
            checkCurlStatus(myStatus, PLUS_FILE_LINE);
            if (myMaxFd != -1) {
                select(myMaxFd+1, &myReadHandles, &myWriteHandles, &myExceptHandles, &tv);
            }
        }
        int myRunningHandles;
        do {
            myStatus = curl_multi_perform(_myCurlMultiHandle, &myRunningHandles);
            checkCurlStatus(myStatus, PLUS_FILE_LINE);
            DB(AC_TRACE << "curl multi perform returned " << myRunningHandles << " running handles. More Data..." << endl);

            handleMessages();

        } while (myStatus == CURLM_CALL_MULTI_PERFORM);
        return myRunningHandles;
    }

    void 
    RequestManager::getRequest(const std::string & theUrl, const RequestCallbackPtr theCB) {
        RequestPtr myRequest = RequestPtr(new Request(theUrl));
        myRequest->setOnDoneCallback(theCB);
        myRequest->get();
        performRequest(myRequest);
    }

    void 
    RequestManager::postRequest(const std::string & theUrl, const std::string & theData, const RequestCallbackPtr theCB) {
        RequestPtr myRequest = RequestPtr(new Request(theUrl));
        myRequest->setOnDoneCallback(theCB);
        myRequest->post(theData);
        performRequest(myRequest);
    }
    
    void 
    RequestManager::putRequest(const std::string & theUrl, const std::string & theData, const RequestCallbackPtr theCB) {
        RequestPtr myRequest = RequestPtr(new Request(theUrl));
        myRequest->setOnDoneCallback(theCB);
        myRequest->put(theData);
        performRequest(myRequest);
    }

    void 
    RequestManager::deleteRequest(const std::string & theUrl, const RequestCallbackPtr theCB) {
        RequestPtr myRequest = RequestPtr(new Request(theUrl));
        myRequest->setOnDoneCallback(theCB);
        myRequest->http_delete();
        performRequest(myRequest);
    }

    void
    RequestManager::getAllRequest(const std::string & theBaseURL, const std::vector<std::string> & theURLLastPartList,
                                  const RequestCallbackPtr theOneReadyCB, const RequestCallbackPtr theAllReadyCB) {
        RequestPtr myNextRequest;
        for (int i = theURLLastPartList.size() - 1; i >= 0 ; --i) {
            std::string myUrl = theBaseURL + "/" + theURLLastPartList[i];
            SequenceRequestPtr myRequest = SequenceRequestPtr(new SequenceRequest(*this, myUrl));
            myRequest->setOnDoneCallback(theOneReadyCB);
            myRequest->get();
            if (myNextRequest) {
                myRequest->setNextRequest(myNextRequest);
            } else {
                myRequest->setOnAllDoneCallback(theAllReadyCB);
            }
            myNextRequest = myRequest;
        }
        if (myNextRequest) {
            performRequest(myNextRequest);
        }
    }
}
