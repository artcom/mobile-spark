// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2012, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __


#include "SocketAdapter.h"
#include "MultiAdapter.h"

namespace masl {
namespace networking {
    
std::map<curl_socket_t, SocketAdapterPtr> SocketAdapter::_allSockets;

SocketAdapter::SocketAdapter(CURLM * theCurlMultihandle) :
    _multiAdapter(NetAsyncSingleton::get().na()->getMultiAdapter()),
    boost_socket(_multiAdapter->io),
    _readyState(0),
    read_in_progress(false),
    write_in_progress(false)
{ 
    AC_DEBUG << "creating socket " << this;
    boost_socket.open(boost::asio::ip::tcp::v4());
    boost::asio::ip::tcp::socket::non_blocking_io non_blocking_io(true);
    boost_socket.io_control(non_blocking_io);
    AC_TRACE << "         socket is " << native();
};

SocketAdapter::~SocketAdapter() {
    AC_TRACE << "SocketAdapter::DTOR socket " << this;
};

void SocketAdapter::handleOperations(SocketAdapterPtr s,
                                curl_socket_t theCurlSocket) 
    {
        AC_TRACE << "handleOperations: socket " << theCurlSocket << " is " << s->getReadyState();
        if (!s->boost_socket.is_open()) {
            return;
        }
        // NOTE: this will be called from one of io_service's threads
        switch (s->getReadyState()) {
            case CURL_POLL_OUT:
                if (s->write_in_progress == false) {
                    s->write_in_progress = true;
                    AC_TRACE << "queuing write " << s->native();
                    s->boost_socket.async_write_some(
                            boost::asio::null_buffers(),
                            s->_multiAdapter->_strand.wrap(
                            boost::bind(&SocketAdapter::handleWrite, s,
                                boost::asio::placeholders::error)));
                }
                break;
            case CURL_POLL_IN:
                if (s->read_in_progress == false) {
                    s->read_in_progress = true;
                    AC_TRACE << "queuing read " << s->native();
                    s->boost_socket.async_read_some(
                            boost::asio::null_buffers(),
                            s->_multiAdapter->_strand.wrap(
                            boost::bind(&SocketAdapter::handleRead, s,
                                boost::asio::placeholders::error)));

                }
                break;
            case CURL_POLL_REMOVE:
                // we don't need to do anything here. We'll keep the connection open 
                // so it can be reused by later clients
                break;
            default:
                throw masl::Exception("Unknown Socket State "+s->getReadyState()); 
        }
}


void SocketAdapter::handleRead(const boost::system::error_code& error) {
    Ptr self(shared_from_this());
    // NOTE: this will be called from one of io_service's threads
    AC_TRACE << "  doing read " << this << " on socket " << native() << " with error " << error;
    read_in_progress = false;
    if (error != 0) {
        if (error == boost::asio::error::operation_aborted) {
            AC_TRACE << "Read aborted";
        } else {
            AC_TRACE << "Read aborted with unknown error " << error;
        }
        return;
    }
    int i;
    CURLMcode myStatus = curl_multi_socket_action(_multiAdapter->_curlMulti,
                                                  native(), CURL_CSELECT_IN,
                                                  &i);
    
    MultiAdapter::checkCurlStatus(myStatus, PLUS_FILE_LINE);
    AC_TRACE << "   done read " << this << " socket " << native();
    SocketAdapter::handleOperations(self, native());
}

void SocketAdapter::handleWrite(const boost::system::error_code& error) {
    Ptr self(shared_from_this());
    // NOTE: this will be called from one of io_service's threads
    AC_TRACE << "  doing write " << this << " with error " << error << " socket is " << native();
    write_in_progress = false;
    if (boost_socket.is_open() && error == 0) {
        int i;
        CURLMcode myStatus = curl_multi_socket_action(_multiAdapter->_curlMulti,
                                                      native(),
                                                      CURL_CSELECT_OUT,
                                                      &i);
        
        MultiAdapter::checkCurlStatus(myStatus, PLUS_FILE_LINE);
        AC_TRACE << "   done write " << this;
    }

    if (boost_socket.is_open() && error == 0) {
        SocketAdapter::handleOperations(self, native());
    } 
}

}
}

