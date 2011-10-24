// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#ifndef _included_mobile_spark_I18nConstants_
#define _included_mobile_spark_I18nConstants_

#include <masl/Exception.h>

namespace spark {
    DECLARE_EXCEPTION(LanguageNotFoundException, masl::Exception)

    enum LANGUAGE {DE, EN, NO_LANGUAGE = 1000};

    class I18nConstants {
    private:
        static const size_t NUM_LANGUAGES;
    public:
        static const char* LANGUAGE_STRINGS[];
        static LANGUAGE getLanguageId(const std::string & theString);
    };
};
#endif
