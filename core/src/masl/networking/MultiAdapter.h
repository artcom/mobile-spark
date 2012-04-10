// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2012, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _ac_mobile_masl_networking_multiadapter_h_included_
#define _ac_mobile_masl_networking_multiadapter_h_included_


#include <set>
#include <curl/curl.h>
#include <boost/asio.hpp>

#include <masl/Logger.h>

#include "../NetAsync.h"

namespace masl {
    namespace networking {
        
        class MultiAdapter {
        public:
            friend class SocketAdapter;
            MultiAdapter(boost::asio::io_service & theIOService);
            virtual ~MultiAdapter();
            void addClient(ClientPtr theClient);
            void removeClient(ClientPtr theClient);
            void processCompleted();
            void processCallbacks();
            curl_socket_t openSocket(); 
            
            static void checkCurlStatus(CURLMcode theStatusCode, const std::string & theWhere); 
            void shutdown();
        protected:
            void setSocketInfo(curl_socket_t s, void * data);
        private:
            MultiAdapter();
            // curl stuff
            CURLM * _curlMulti;
            boost::shared_ptr<boost::asio::deadline_timer> timeout_timer;
            
            std::set<ClientPtr> _allClients;
            static int curl_socket_callback(CURL *easy, curl_socket_t s, int action, void *userp, void *socketp); 
            static int curl_timer_callback(CURLM *multi,  long timeout_ms, MultiAdapter * self); 
            void onTimeout(const boost::system::error_code& error);
            
            // Boost IO stuff
            
            // owned by NetAsync
            boost::asio::io_service & io;
            boost::asio::io_service::strand _strand;
            
        };
        
        //typedef masl::Ptr<MultiAdapter> MultiAdapterPtr;
        
    }
}

#endif
