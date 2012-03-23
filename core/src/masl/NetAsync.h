// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2012, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __


#ifndef _ac_mobile_masl_net_async_h_included_
#define _ac_mobile_masl_net_async_h_included_

#include "networking/MultiAdapter.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Singleton.h"

namespace masl {

	class NetAsync {
    public:
        typedef masl::Ptr<boost::thread> AsioThreadPtr;
        typedef boost::function<void()> onFrameHandler;

        NetAsync();
        virtual ~NetAsync();
        boost::asio::io_service & io_service();

        virtual void onFrame();

        void registerHandler(const void * theInstance, onFrameHandler & theHandler) {
            _onFrameHandlers.insert(std::make_pair(theInstance, theHandler));
        }
        void unregisterHandler(const void * theInstance) {
            std::map<const void*, onFrameHandler>::iterator it = _onFrameHandlers.find(theInstance);
            if (it != _onFrameHandlers.end()) {
                _onFrameHandlers.erase(it);
            }
        }
        networking::MultiAdapter & getMultiAdapater() { return _multiAdapter; };

    private:
        std::map<const void*, onFrameHandler> _onFrameHandlers;  
        void run(std::size_t thread_pool_size);
        /// The io_service used to perform asynchronous operations.
        boost::asio::io_service io;
        // fictional work item to prevent our io_service from being out of work and terminating
        boost::shared_ptr<boost::asio::io_service::work> keep_busy;
        networking::MultiAdapter _multiAdapter;


    private:
        AsioThreadPtr _myAsioThread;
	};

    typedef masl::Ptr<NetAsync> NetAsyncPtr;

    class NetAsyncSingleton : public masl::Singleton<NetAsyncSingleton> {
        public:
            virtual ~NetAsyncSingleton();
            void setNetAsync(NetAsyncPtr theNetAsync);
            const NetAsyncPtr & ap() const { return _myNetAsync;};

        private:
            NetAsyncPtr _myNetAsync;
    };
}

#endif
