#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdarg.h>

typedef enum {
	ERR=0,
	OK=1,
	OK_CONT=2,
	DATA=3,
	INFO=4,
	DEBUG=5,
} dprint_t;

#define DPRINT_LEVEL (INFO)

int dprint(dprint_t type, const char *format, ...);

#endif
