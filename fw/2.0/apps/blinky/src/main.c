
#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"

#define OSTUR_TASK_PRI         (10)
#define OSTUR_STACK_SIZE       (256)
struct os_task ostur_task;
os_stack_t ostur_task_stack[OSTUR_STACK_SIZE];

void ostur_task_fn(void *arg) {
    uint8_t count = 0;

    hal_gpio_init_out(LED_0, 0);
    hal_gpio_init_out(LED_1, 0);
    hal_gpio_init_out(LED_2, 0);

    while (1) {

        os_time_delay(OS_TICKS_PER_SEC/4);

        hal_gpio_write(LED_0, (count >> 0) & 0x1);
        hal_gpio_write(LED_1, (count >> 1) & 0x1);
        hal_gpio_write(LED_2, (count >> 2) & 0x1);

        count++;
    }
}

int
main(int argc, char **argv)
{
    sysinit();

    os_task_init(
        &ostur_task,
        "ostur_task",
        ostur_task_fn,
        NULL,
        OSTUR_TASK_PRI,
        OS_WAIT_FOREVER,
        ostur_task_stack,
        OSTUR_STACK_SIZE);

    while(1) {
        os_eventq_run(os_eventq_dflt_get());
    }

    assert(0);

    return 0;
}

