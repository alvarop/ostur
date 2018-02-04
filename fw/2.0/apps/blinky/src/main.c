
#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"

int
main(int argc, char **argv)
{
    uint8_t count = 0;
    sysinit();

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
    assert(0);

    return 0;
}

