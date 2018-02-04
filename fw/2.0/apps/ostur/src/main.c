#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "console/console.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#include "i2c/i2c.h"
#include "tca95xxa/tca95xxa.h"
#include "sht3x/sht3x.h"

#if MYNEWT_VAL(USE_BLE)
#include <host/ble_hs.h>
#endif

#define OSTUR_TASK_PRI         (10)
#define OSTUR_STACK_SIZE       (256)
struct os_task ostur_task;
os_stack_t ostur_task_stack[OSTUR_STACK_SIZE];

static uint16_t timestamp;

#if MYNEWT_VAL(USE_BLE)
#define BEACON_MAGIC 0xDA7A

typedef struct {
    uint16_t magic;
    uint16_t timestamp;
    uint8_t dummy[12];
} __attribute__((packed)) ble_beacon_t;

static ble_beacon_t beacon_data;

static void ble_app_advertise();

static void ble_app_set_addr(void) {
    ble_addr_t addr;
    int rc;

    rc = ble_hs_id_gen_rnd(1, &addr);
    assert(rc == 0);

    rc = ble_hs_id_set_rnd(addr.val);
    assert(rc == 0);
}

static void ble_app_advertise() {
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    beacon_data.timestamp = timestamp;

    fields = (struct ble_hs_adv_fields){ 0 };
    rc = ble_eddystone_set_adv_data_uid(&fields, &beacon_data);
    assert(rc == 0);

    adv_params = (struct ble_gap_adv_params){ 0 };
    rc = ble_gap_adv_start(
        BLE_OWN_ADDR_RANDOM,
        NULL,
        500,
        &adv_params,
        NULL,
        NULL);
    assert(rc == 0);
}

static void ble_app_on_sync(void) {
    /* Generate a non-resolvable private address. */
    ble_app_set_addr();
}
#endif

void ostur_task_fn(void *arg) {
    hal_gpio_init_out(LED_0_PIN, 0);
    hal_gpio_init_out(TCA_NRST_PIN, 1);

    console_printf("Ostur Controller v2.0\n");

    i2c_init(0, SDA_PIN, SCL_PIN, 100);

    #if MYNEWT_VAL(USE_BLE)
        beacon_data.magic = 0xDA7A;
        beacon_data.timestamp = 0;
    #endif


    tca95xxa_set_channel(0x70, 3);

    sht3x_init(SHT3x_ADDR);

    while (1) {

        os_time_delay(OS_TICKS_PER_SEC);

        ble_app_advertise();
        hal_gpio_toggle(LED_0_PIN);

        timestamp++;
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

#if MYNEWT_VAL(USE_BLE)
     ble_hs_cfg.sync_cb = ble_app_on_sync;
#endif

    while(1) {
        os_eventq_run(os_eventq_dflt_get());
    }

    assert(0);

    return 0;
}

