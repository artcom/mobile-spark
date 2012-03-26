// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2012, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __


#ifndef _ac_mobile_masl_networking_client_h_included_
#define _ac_mobile_masl_networking_client_h_included_

#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <curl/curl.h>

#include <masl/Callback.h>
#include <masl/ThreadLock.h>


namespace masl {
namespace networking {

    class SocketAdapter;
    typedef masl::Ptr<SocketAdapter> SocketAdapterPtr;

    class Client : public boost::enable_shared_from_this<Client> {
        public:
            CURL * _curlHandle;
        private:
            std::vector<char>   _myErrorBuffer;
            std::vector<char> _privateResponseBuffer; // filled in io_service thread, emptied in application thread
            masl::ThreadLock _lockResponseBuffer; // lock for _privateResponseBuffer;
            std::vector<char> _myResponseBlock; // used only in application thread.
            bool _continueFlag;
        public:
            /// creates a new HttpClient
            Client(const std::string &url, const bool verbose, const long connecttimeout);
            virtual ~Client();
            curl_socket_t getCurlSocket();
            void performSync();
            void performAsync();
            void onDone(CURLcode result);
            void onProgress();
            std::string getResponseString() const;
            std::vector<char> getResponseBlock() const;
            void abort() { _continueFlag = false; };
            std::string debugIdentifier;
            
        private:
            void checkCurlStatus(CURLcode theStatusCode, const std::string & theWhere); 

            //TODO:
            //setOnError, setOnSuccess
            masl::CallbackPtr onErrorCB;
            masl::CallbackPtr onSuccessCB;
            masl::CallbackPtr onProgressCB;

            //TODO: set from outside?
            SocketAdapterPtr _socketAdapter;

            size_t writeFunction(const unsigned char *ptr, size_t size);
            static size_t _writeFunction(unsigned char *ptr, size_t size, size_t nmemb, Client *self) {
                AC_DEBUG << "calling writefunction for " << self;
                if (self) {
                    return self->writeFunction(ptr, size*nmemb);
                } else {
                    return 0;
                }
            };

            curl_socket_t openSocket(curlsocktype purpose, struct curl_sockaddr *addr);
            static curl_socket_t _openSocket(Client *self, curlsocktype purpose, struct curl_sockaddr *addr) {
                return self->openSocket(purpose, addr);
            };
            
            static int _closeSocket(Client *self, curl_socket_t item);
    };

    typedef masl::Ptr<Client> ClientPtr;
} 
}
#endif
