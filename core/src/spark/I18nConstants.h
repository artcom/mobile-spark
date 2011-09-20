#ifndef _included_mobile_spark_I18nConstants_
#define _included_mobile_spark_I18nConstants_

#include <masl/Exception.h>

namespace spark {
    DEFINE_EXCEPTION(LanguageNotFoundException, masl::Exception)

    enum LANGUAGE {DE, EN, NO_LANGUAGE = 1000};
    const char* LANGUAGE_STRINGS[] = {"de", "en"};
    const size_t NUM_LANGUAGES = 2;
    LANGUAGE getLanguageId(const std::string & theString);
};
#endif
