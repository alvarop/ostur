#include <assert.h>
#include <string.h>

#include <sysinit/sysinit.h>
#include <os/os.h>
#include <console/console.h>
#include <bsp/bsp.h>
#include <hal/hal_gpio.h>
#include <i2c/i2c.h>
#include <sht3x/sht3x.h>
#include <simple_adc/simple_adc.h>

#if MYNEWT_VAL(USE_BLE)
#include <host/ble_hs.h>
#endif

#define OSTUR_TASK_PRI         (20)
#define OSTUR_STACK_SIZE       (256)
struct os_task ostur_task;
os_stack_t ostur_task_stack[OSTUR_STACK_SIZE];

static volatile uint16_t timestamp;

#if MYNEWT_VAL(USE_BLE)
#define BEACON_MAGIC 0x0579

typedef struct {
    uint16_t magic;
    uint32_t device_id;
    uint16_t timestamp;
    int16_t temperature;
    int16_t humidity;
    uint16_t batt;
    uint16_t flags;
} __attribute__((packed)) ble_beacon_t;

static ble_beacon_t beacon_data;

static void ble_app_advertise();

static int gap_event_cb(struct ble_gap_event *event, void *arg) {
    switch (event->type) {

    case BLE_GAP_EVENT_ADV_COMPLETE:
        // ble_app_advertise();
        break;
    }
    return 0;
}

static void ble_app_set_addr(void) {
    ble_addr_t addr;
    int rc;

    rc = ble_hs_id_gen_rnd(1, &addr);
    assert(rc == 0);

    rc = ble_hs_id_set_rnd(addr.val);
    assert(rc == 0);
}

// Use a BLE_CH_ADV_TIME_MS long beacon to send each individual
// channel's data
static void ble_app_advertise() {
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    beacon_data.device_id = NRF_FICR->DEVICEADDR[0];
    beacon_data.timestamp = timestamp;
    beacon_data.flags = 0;

    fields = (struct ble_hs_adv_fields){ 0 };
    fields.mfg_data_len = 1;
    fields.mfg_data = (uint8_t[]){0x05};

    rc = ble_eddystone_set_adv_data_uid(&fields, &beacon_data, 0);
    assert(rc == 0);

    adv_params = (struct ble_gap_adv_params){ 0 };
    rc = ble_gap_adv_start(
        BLE_OWN_ADDR_RANDOM,
        NULL,
        MYNEWT_VAL(BLE_CH_ADV_TIME_MS),
        &adv_params,
        gap_event_cb,
        NULL);
    assert(rc == 0);

}

static void ble_app_on_sync(void) {
    /* Generate a non-resolvable private address. */
    ble_app_set_addr();
}
#endif

#define BATT_ADC_SAMPLES 16

void ostur_task_fn(void *arg) {

    console_printf("Ostur Sensor v1.0\n");

    #if MYNEWT_VAL(USE_BLE)
        beacon_data.magic = BEACON_MAGIC;
        beacon_data.timestamp = 0;
    #endif

    console_printf("Detecting HT Sensors\n");
    int32_t rval;

    rval = sht3x_init(SHT3x_ADDR);
    console_printf("0x%02X - %ld\n", SHT3x_ADDR, rval);
    if (rval != 0) {
        hal_gpio_write(LED2_PIN, 1);
        hal_gpio_write(LED1_PIN, 1);
        while(1){
            __asm("NOP;");
        }
    }

    os_time_delay(OS_TICKS_PER_SEC * 5);
    while (1) {
        int32_t batt = 0;
        int32_t mv;

        hal_gpio_write(LED2_PIN, 1);

        simple_adc_init();
        simple_adc_init_ch(0, NRF_SAADC_INPUT_VDD);
        for(uint8_t sample = 0; sample < BATT_ADC_SAMPLES; sample++){
            simple_adc_read_ch(0, &mv);
            batt += mv;
        }
        simple_adc_uninit();
        beacon_data.batt = (uint16_t)(batt/BATT_ADC_SAMPLES);

        rval = sht3x_read(SHT3x_ADDR, &beacon_data.temperature, &beacon_data.humidity);
        console_printf("t:%d h:%d\n", beacon_data.temperature, beacon_data.humidity);


#if MYNEWT_VAL(USE_BLE)

        ble_app_advertise();
#endif

        hal_gpio_write(LED2_PIN, 0);

        timestamp++;

        os_time_delay(OS_TICKS_PER_SEC * MYNEWT_VAL(SAMPLE_RATE_S));
    }
}


int
main(int argc, char **argv)
{
    sysinit();

    hal_gpio_init_out(LED1_PIN, 0);
    hal_gpio_init_out(LED2_PIN, 0);

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
