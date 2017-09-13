#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <stdbool.h>
#include <stdint.h>

int32_t controller_init();
void controller_process();
int32_t controller_enable(bool enabled);
int32_t controller_autoconfig();

#endif
