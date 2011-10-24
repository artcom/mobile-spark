// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

#include "I18nConstants.h"
#include <masl/string_functions.h>
namespace spark {

    DEFINE_EXCEPTION(LanguageNotFoundException, masl::Exception)

    const size_t I18nConstants::NUM_LANGUAGES = 2;
    const char* I18nConstants::LANGUAGE_STRINGS[I18nConstants::NUM_LANGUAGES] = {"de", "en"};

    LANGUAGE
    I18nConstants::getLanguageId(const std::string & theString) {
        for (size_t i = 0; i < NUM_LANGUAGES; ++i) {
            if (theString == LANGUAGE_STRINGS[i]) {
                return (LANGUAGE)i;
            }
        }
        throw LanguageNotFoundException("Language " + theString + " is not found ", PLUS_FILE_LINE);
    }
}

