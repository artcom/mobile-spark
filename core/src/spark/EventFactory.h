// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_EventFactory_
#define _included_mobile_spark_EventFactory_

#include <masl/Singleton.h>
#include <masl/Exception.h>

#include "Event.h"

namespace spark {

    DECLARE_EXCEPTION(EventFactoryException, masl::Exception);

    class EventFactory : public masl::Singleton<EventFactory> {
    public:
        EventFactory();
        virtual ~EventFactory();
        typedef EventPtr (*CreateEventCallback)( const masl::XMLNodePtr theXMLNode);
    private:
        typedef std::map<const std::string, CreateEventCallback> CallbackMap;
    public:
        bool registerEvent(const std::string & theEventName,
                        const CreateEventCallback theCreateFn);
        bool unregisterEvent(const std::string & theEventName);
        EventPtr createEvent(const std::string & theEvent) const;

    private:
        CallbackMap _myCreateCallbackMap;
        void setupFactory();
    };
};
#endif
