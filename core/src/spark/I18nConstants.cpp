#include "I18nConstants.h"
#include <masl/string_functions.h>
namespace spark {
    LANGUAGE
    getLanguageId(const std::string & theString) {
        for (size_t i = 0; i < NUM_LANGUAGES; ++i) {
            if (theString == LANGUAGE_STRINGS[i]) {
                return (LANGUAGE)i;
            }
        }
        throw LanguageNotFoundException("Language " + theString + " is not found ", PLUS_FILE_LINE);
    }
}

