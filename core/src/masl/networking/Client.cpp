// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2012, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "Client.h"

#include <string>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <masl/AutoLocker.h>

#include "SocketAdapter.h"
#include "MultiAdapter.h"
#include "../NetAsync.h"

using namespace std;

namespace masl {
namespace networking {

    Client::Client(const std::string &url, const bool verbose, const long connecttimeout) :
        _curlHandle(0),
        _continueFlag(true)
    {
        _curlHandle = curl_easy_init();
        AC_DEBUG << "curl init " << curl_version();

        CURLcode myStatus;
        myStatus = curl_easy_setopt(_curlHandle, CURLOPT_ERRORBUFFER, 
                std::string(_myErrorBuffer.begin(), _myErrorBuffer.end()).c_str());
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
        myStatus = curl_easy_setopt(_curlHandle, CURLOPT_PRIVATE, this);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
        
        myStatus = curl_easy_setopt(_curlHandle, CURLOPT_WRITEFUNCTION, &Client::_writeFunction);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
        myStatus = curl_easy_setopt(_curlHandle, CURLOPT_WRITEDATA, this);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
        
        myStatus = curl_easy_setopt(_curlHandle, CURLOPT_OPENSOCKETFUNCTION, &Client::_openSocket);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
        myStatus = curl_easy_setopt(_curlHandle, CURLOPT_OPENSOCKETDATA, this);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);

        //TODO: needs newer curl version
        //myStatus = curl_easy_setopt(_curlHandle, CURLOPT_CLOSESOCKETFUNCTION, &Client::_closeSocket);
        //checkCurlStatus(myStatus, PLUS_FILE_LINE);
        //myStatus = curl_easy_setopt(_curlHandle, CURLOPT_CLOSESOCKETDATA, this);
        //checkCurlStatus(myStatus, PLUS_FILE_LINE);

        //CURLcode myStatus = curl_easy_setopt(_curlHandle, CURLOPT_URL, url);
        //checkCurlStatus(myStatus, PLUS_FILE_LINE);

        if (verbose) {
            myStatus = curl_easy_setopt(_curlHandle, CURLOPT_VERBOSE, 1);
            checkCurlStatus(myStatus, PLUS_FILE_LINE);
        }

        myStatus = curl_easy_setopt(_curlHandle, CURLOPT_CONNECTTIMEOUT, connecttimeout);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
    }

    void 
    Client::performSync() {
        CURLcode myStatus = curl_easy_perform(_curlHandle);
        onDone(myStatus);
    }

    void
    Client::performAsync() {
        AC_DEBUG << "starting request " << this;
        NetAsyncSingleton::get().na()->getMultiAdapter()->addClient(shared_from_this());
    }

    Client::~Client()
    {
        AC_DEBUG << "~Client " << this;
        CURLcode myStatus = curl_easy_setopt(_curlHandle, CURLOPT_OPENSOCKETDATA, 0);
        checkCurlStatus(myStatus, PLUS_FILE_LINE);
    }
   
    void
    Client::checkCurlStatus(CURLcode theStatusCode, const std::string & theWhere) {
        if (theStatusCode != CURLE_OK) {
            throw masl::Exception(string(_myErrorBuffer.begin(), _myErrorBuffer.end()), theWhere);
        }
    }

    std::string
    Client::getResponseString() const {
        return std::string(_myResponseBlock.begin(), _myResponseBlock.end());
    }

    std::vector<char> 
    Client::getResponseBlock() const {
        return _myResponseBlock;
    }
    
    void 
    Client::onProgress() {
        bool newDataReceived = false;
        {
            AutoLocker<ThreadLock> myLocker(_lockResponseBuffer);        
            if (_privateResponseBuffer.size() > 0) {
                _myResponseBlock.insert(_myResponseBlock.end(), 
                                        _privateResponseBuffer.begin(),
                                        _privateResponseBuffer.end());
                _privateResponseBuffer.clear();
                newDataReceived = true;
            }
        }
        //TODO: check correctness here
        //if (newDataReceived && onProgress) {
        if (newDataReceived) {
            AC_TRACE << "calling onProgress for " << this;
            //_continueFlag = (*onProgressCB)(/*TODO*/);
        }
    }

    void
    Client::onDone(CURLcode result) {
        {
            AutoLocker<ThreadLock> myLocker(_lockResponseBuffer);        
            _myResponseBlock.insert(_myResponseBlock.end(), 
                                    _privateResponseBuffer.begin(),
                                    _privateResponseBuffer.end());
            _privateResponseBuffer.clear();
        }

        AC_DEBUG << "onDone. CURLcode is " << result << " for " << this;
        AC_DEBUG << "error string:" << std::string(_myErrorBuffer.begin(), _myErrorBuffer.end());
        if (result == CURLE_OK) {
            if (onSuccessCB) {
                (*onSuccessCB)();
                AC_DEBUG << "called success";
            }
        } else {
            if (onErrorCB) {
                (*onErrorCB)();
            }
        }
    }

    size_t 
    Client::writeFunction(const unsigned char *ptr, size_t size) {
        // NOTE: this will be called from one of io_service's threads
        AutoLocker<ThreadLock> myLocker(_lockResponseBuffer); 
        
        //TODO: fix
        //copy(theData, theData + size, back_inserter(_privateResponseBuffer));
        return _continueFlag ? size : 0;
    }

    curl_socket_t 
    Client::openSocket(curlsocktype purpose, struct curl_sockaddr *addr) {
        AC_DEBUG << "curl requesting open socket";
        return NetAsyncSingleton::get().na()->getMultiAdapter()->openSocket();
    }
    
    int Client::_closeSocket(Client *self, curl_socket_t item) {
        AC_DEBUG << "closing socket " << item;
        SocketAdapterPtr s = SocketAdapter::find(item);
        if (s) {
            s->close();
        }
        return 0;
    }
}
}
