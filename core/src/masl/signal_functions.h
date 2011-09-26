#ifndef _included_mobile_masl_signal_functions_
#define _included_mobile_masl_signal_functions_

#include <signal.h>

namespace masl {

void initSignalHandling();
void traceSignal( int theSignal );

};

#endif
