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

