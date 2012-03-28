// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2012, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __


#include "MultiAdapter.h"

#include <masl/Logger.h>

#include "Client.h"
#include "SocketAdapter.h"

using namespace masl;

namespace masl {
namespace networking {

MultiAdapter::MultiAdapter(boost::asio::io_service & theIOService) :
    io(theIOService),
    _strand(theIOService)
{
    _curlMulti = curl_multi_init(); 
    CURLMcode myStatus = curl_multi_setopt(_curlMulti, CURLMOPT_SOCKETFUNCTION,
                                           &MultiAdapter::curl_socket_callback);
    
    checkCurlStatus(myStatus, PLUS_FILE_LINE);
    myStatus = curl_multi_setopt(_curlMulti, CURLMOPT_SOCKETDATA, this); 
    checkCurlStatus(myStatus, PLUS_FILE_LINE);

    myStatus = curl_multi_setopt(_curlMulti, CURLMOPT_TIMERFUNCTION,
                                 &MultiAdapter::curl_timer_callback);
    
    checkCurlStatus(myStatus, PLUS_FILE_LINE);
    myStatus = curl_multi_setopt(_curlMulti, CURLMOPT_TIMERDATA, this); 
    checkCurlStatus(myStatus, PLUS_FILE_LINE);

}
    
MultiAdapter::~MultiAdapter() {
    AC_TRACE << "~MultiAdapter done";
}

curl_socket_t MultiAdapter::openSocket() {
    SocketAdapterPtr s(new SocketAdapter(_curlMulti));
    SocketAdapter::add(s);
    return s->native();
}

void MultiAdapter::shutdown() {
    AC_TRACE << "MultiAdapter::shutdown";
    
    while (!_allClients.empty()) {
        (*_allClients.begin())->onDone(CURLE_ABORTED_BY_CALLBACK );
        removeClient(*_allClients.begin());
    }
    
    SocketAdapter::abort();
    CURLMcode myStatus = curl_multi_cleanup(_curlMulti); 
    checkCurlStatus(myStatus, PLUS_FILE_LINE);
    if (timeout_timer) {
        timeout_timer->cancel();
    }
    AC_TRACE << "MultiAdapter::shutdown done";
}

void MultiAdapter::checkCurlStatus(CURLMcode theStatusCode,
                                   const std::string & theWhere) 
{
    if (theStatusCode != CURLM_OK) {
        throw masl::Exception(curl_multi_strerror(theStatusCode), theWhere);
    }
}

void  MultiAdapter::addClient(ClientPtr theClient) { 
    AC_DEBUG << "adding client " << theClient;
    CURLMcode myStatus = curl_multi_add_handle(_curlMulti,
                                               theClient->_curlHandle);
    checkCurlStatus(myStatus, PLUS_FILE_LINE);
    _allClients.insert(theClient);
    //int i;
    //myStatus = curl_multi_socket_action(_curlMulti, 0, 0, &i);
    //checkCurlStatus(myStatus, PLUS_FILE_LINE);
}
    
void MultiAdapter::removeClient(ClientPtr theClient ){ 
    AC_DEBUG << "removeClient client " << theClient;
    CURLMcode myStatus = curl_multi_remove_handle(_curlMulti,
                                                  theClient->_curlHandle); 
    checkCurlStatus(myStatus, PLUS_FILE_LINE);
    _allClients.erase(theClient);
};


int MultiAdapter::curl_socket_callback(CURL *easy, /* easy handle */   
                                       curl_socket_t theCurlSocket, /* socket */   
                                       int action, /* see values below */   
                                       void *userp, /* private callback pointer */   
                                       void *socketp) /* private socket pointer */ 
{
    Client * curClient = 0;
    curl_easy_getinfo(easy, CURLINFO_PRIVATE, &curClient);
    AC_DEBUG << "Curl Socket "<< theCurlSocket << " Callback: " << action << " on " << userp << "," << curClient;
    SocketAdapterPtr s = SocketAdapter::find(theCurlSocket);
    if (s) {
        s->setReadyState(action);
        SocketAdapter::handleOperations(s, theCurlSocket);
    }
    return 0;
};

int MultiAdapter::curl_timer_callback(CURLM *multi,
                                      long timeout_ms,
                                      MultiAdapter * self) 
{
    AC_TRACE << "multi_timer_cb: Setting timeout to " << timeout_ms << " ms";
    if ( ! self->timeout_timer) {
        self->timeout_timer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(self->io)); 
    }
    self->timeout_timer->expires_from_now(boost::posix_time::milliseconds(timeout_ms));
    // self->onTimeout(boost::asio::error::operation_aborted);
    self->timeout_timer->async_wait(self->_strand.wrap(bind(&MultiAdapter::onTimeout, self, boost::asio::placeholders::error)));
    return 0;
}

void MultiAdapter::onTimeout(const boost::system::error_code& error) {
    AC_TRACE << "onTimeout " << error;
    if (error != 0) {
        return;
    }
    int i;
    CURLMcode myStatus = curl_multi_socket_action(_curlMulti, CURL_SOCKET_TIMEOUT, 0, &i);
    checkCurlStatus(myStatus, PLUS_FILE_LINE);
};

void
MultiAdapter::processCallbacks() {
    for (std::set<ClientPtr >::iterator it = _allClients.begin(); it != _allClients.end(); ++it) {
        (*it)->onProgress();
    }
};

void MultiAdapter::setSocketInfo(curl_socket_t s, void * data) {
    CURLMcode myStatus = curl_multi_assign(_curlMulti, s, data);
    checkCurlStatus(myStatus, PLUS_FILE_LINE);
};

void MultiAdapter::processCompleted() {
    // take care of completed requests
    int myMessageCount = 0;
    CURLMsg * myMessage = 0;
    do {
        myMessage = curl_multi_info_read(_curlMulti, &myMessageCount);
        if (myMessage) {
            CURL * myEasyHandle = myMessage->easy_handle;
            Client * curClient = 0;
            curl_easy_getinfo(myEasyHandle, CURLINFO_PRIVATE, &curClient);
            if (myMessage->msg == CURLMSG_DONE) {
                AC_DEBUG << "calling onDone for " << curClient;
                curClient->onDone(myMessage->data.result);
                removeClient(curClient->shared_from_this());
            } else {
                throw masl::Exception("Unknown CURL message encountered");
            }
        }
    } while (myMessage);
};

}
}

