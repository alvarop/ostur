#include <stdio.h>
#include "debug.h"

static const char *dprint_headers[] = {
	"ERR ",
	"OK ",
	"",
	"DATA ",
	"INFO ",
	"DEBUG ",
};

int dprint(dprint_t type, const char *format, ...) {
	int rval = 0;
	if(type <= DPRINT_LEVEL) {
		if(type < sizeof(dprint_headers)/sizeof(char *)) {
			// Print line header
			fputs(dprint_headers[type], stdout);

			// Print rest of stuff
			va_list args;
			va_start(args, format);
			rval = vprintf(format, args);
			va_end(args);
		}
	}

	return rval;
}
