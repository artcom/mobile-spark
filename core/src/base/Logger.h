/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// These coded instructions, statements, and computer programs contain
// proprietary information of ART+COM AG Berlin, and are copy protected
// by law. They may be used, modified and redistributed under the terms
// of GNU General Public License referenced below.
//
// Alternative licensing without the obligations of the GPL is
// available upon request.
//
// GPL v3 Licensing:
//
// This file is part of the ART+COM Y60 Platform.
//
// ART+COM Y60 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ART+COM Y60 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ART+COM Y60.  If not, see <http://www.gnu.org/licenses/>.
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
*/
#ifndef _ac_mobile_asl_Logger_h_included_
#define _ac_mobile_asl_Logger_h_included_

#include <string>
#include <sstream>

#include "Singleton.h"

namespace asl {
    enum Severity {SEV_PRINT, SEV_FATAL, SEV_ERROR, SEV_WARNING, SEV_INFO, SEV_DEBUG, SEV_TRACE, SEV_DISABLED};
    
    class Logger : public Singleton<Logger> {
        friend class Singleton< Logger >;        
        public:
        Logger();            
        void log(/*asl::Time theTime,*/ Severity theSeverity, const char * theModule, int theId, const std::string & theText);        
    };
    
    /**
    This class is used to collect the output and deliver it to the Logger on destruction
    */    
    class MessagePort {
    public:
        MessagePort(Severity theSeverity, const char * theModule, int theId)
            : mySeverity(theSeverity), myModule(theModule), myId(theId)
        {}
        ~MessagePort() {
            Logger::get().log(/*myTime,*/ mySeverity, myModule, myId, stream.str());
        }
        /* This getter is used in the gcc branch to avoid printing of the first message token
         * as a hex address. I still do not understand what exactly goes wrong when accessing
         * the member directly. However, this solves bug #24.
         */
        inline std::ostringstream & getStream() {
            return stream;
        }
        std::ostringstream stream;
        //const Time myTime;
        const Severity mySeverity;
        const char * myModule;
        const int myId;
    };


#ifdef _WIN32
    #define AC_PRINT const_cast<std::ostream&>( static_cast<const std::ostream&>(asl::MessagePort(asl::SEV_PRINT, __FILE__ ,__LINE__).stream) )
    #define AC_ERROR const_cast<std::ostream&>( static_cast<const std::ostream&>(asl::MessagePort(asl::SEV_ERROR, __FILE__ ,__LINE__).stream) )    
    #define AC_WARNING const_cast<std::ostream&>( static_cast<const std::ostream&>(asl::MessagePort(asl::SEV_WARNING, __FILE__ ,__LINE__).stream) )    
#else
    #define AC_PRINT asl::MessagePort(asl::SEV_PRINT, __FILE__ ,__LINE__).getStream()
    #define AC_ERROR asl::MessagePort(asl::SEV_ERROR, __FILE__ ,__LINE__).getStream()
    #define AC_WARNING asl::MessagePort(asl::SEV_WARNING, __FILE__ ,__LINE__).getStream()
#endif
};
#endif
