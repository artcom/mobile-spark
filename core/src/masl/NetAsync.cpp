// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2012, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __


#include "NetAsync.h"

namespace masl {
    

NetAsyncSingleton::~NetAsyncSingleton() {}

void
NetAsyncSingleton::setNetAsync(NetAsyncPtr theNetAsync) {
    _myNetAsync = theNetAsync;
};




NetAsync::NetAsync() : 
                keep_busy(new boost::asio::io_service::work(io)),
                _multiAdapter(io)
{
    _myAsioThread = AsioThreadPtr(new boost::thread( boost::bind( &NetAsync::run, this, 10) ) );
};

NetAsync::~NetAsync() {
    AC_TRACE << "~NetAsync - canceling all sockets";
    _multiAdapter.shutdown();
    AC_TRACE << "~NetAsync - removing keep_busy";
    keep_busy.reset();
    AC_TRACE << "~NetAsync - waiting for ASIO thread";
    // io.stop();
    _myAsioThread->join();
    AC_TRACE << "~NetAsync done";
};

boost::asio::io_service & 
NetAsync::io_service() {
    return io;
};

void
NetAsync::run(std::size_t thread_pool_size) {
    AC_DEBUG << "starting asio threads " << this;
    
    // Create a pool of threads to run all of the io_services.
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (std::size_t i = 0; i < thread_pool_size; ++i)
    {
        boost::shared_ptr<boost::thread> thread(new boost::thread(
                    boost::bind(&boost::asio::io_service::run, &io)));
        threads.push_back(thread);
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();

    AC_DEBUG << "asio threads terminated " << this;
};

void 
NetAsync::onFrame() {
    std::map<const void*, onFrameHandler>::iterator it;
    for (it = _onFrameHandlers.begin(); it != _onFrameHandlers.end(); ++it) {
        (it->second)();
    }
    _multiAdapter.processCallbacks();
    _multiAdapter.processCompleted();    
};
}
