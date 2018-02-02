#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <stdbool.h>
#include <stdint.h>

int32_t ControllerInit();
void ControllerProcess();
int32_t ControllerEnable(bool enabled);
int32_t ControllerAutoconfig();

#endif
