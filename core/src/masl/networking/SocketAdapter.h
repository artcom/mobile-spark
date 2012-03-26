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

#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <curl/curl.h>

#include <masl/Logger.h>

namespace masl {
namespace networking {

class MultiAdapter; 
typedef masl::Ptr<MultiAdapter> MultiAdapterPtr; 

class SocketAdapter : public boost::noncopyable, public boost::enable_shared_from_this<SocketAdapter> {
    public:
        typedef masl::Ptr<SocketAdapter> Ptr;
        SocketAdapter(CURLM * theCurlMultihandle);
        ~SocketAdapter();
        curl_socket_t native() { return boost_socket.native(); };
        boost::asio::ip::tcp::socket boost_socket;
        int readyState;
        bool read_in_progress;
        bool write_in_progress;
        boost::mutex op_in_progress_;
        void handleRead(const boost::system::error_code& error);
        void handleWrite(const boost::system::error_code& error);
        static void handleOperations(Ptr s, curl_socket_t theCurlSocket);
        static void add(Ptr s) {
            AC_TRACE << "Adding socket_" << s->native();
            _allSockets.insert(std::make_pair(s->native(), s));
        };
        static void release(curl_socket_t t) {
            AC_TRACE << "releasing socket_" << t;
            int n = _allSockets.erase(t);
            AC_DEBUG << "erased " << n << " socket " << t << " from _allSockets. " << _allSockets.size() << " left.";
        };

        static void abort() {
            std::map<curl_socket_t, Ptr>::iterator it = _allSockets.begin();
            for (;it != _allSockets.end(); ++it) {
                AC_TRACE << "aborting socket " << it->second->boost_socket.native() << " for " << it->second;
                it->second->boost_socket.close();
            }
        }
        static Ptr find(curl_socket_t s) {
            std::map<curl_socket_t, Ptr>::iterator it = _allSockets.find(s);
            if (it != _allSockets.end()) {
                return it->second;
            }
            return Ptr();
        }
        void close() {
            curl_socket_t item = boost_socket.native();
            boost_socket.close();
            AC_DEBUG << "closing socket " << item;
            release(item);
            AC_DEBUG << "socket " << item << " closed";
        };
    private:
        SocketAdapter();
        MultiAdapterPtr _multiAdapter;
        static std::map<curl_socket_t, Ptr> _allSockets;
};

typedef SocketAdapter::Ptr SocketAdapterPtr;

}
}
#endif
