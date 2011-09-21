#ifndef _included_mobile_spark_I18nConstants_
#define _included_mobile_spark_I18nConstants_

#include <masl/Exception.h>

namespace spark {

    enum LANGUAGE {DE, EN, NO_LANGUAGE = 1000};
    DEFINE_EXCEPTION(LanguageNotFoundException, masl::Exception)

    class I18nConstants {
    private:
        static const size_t NUM_LANGUAGES;
    public:
        static const char* LANGUAGE_STRINGS[];
        static LANGUAGE getLanguageId(const std::string & theString);
    };
};
#endif
