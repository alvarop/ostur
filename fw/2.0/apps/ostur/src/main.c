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

#define OSTUR_TASK_PRI         (20)
#define OSTUR_STACK_SIZE       (256)
struct os_task ostur_task;
os_stack_t ostur_task_stack[OSTUR_STACK_SIZE];

#define FRIDGE_TASK_PRI         (10)
#define FRIDGE_STACK_SIZE       (64)
struct os_task fridge_task;
os_stack_t fridge_task_stack[FRIDGE_STACK_SIZE];

#define BLINK_TASK_PRI         (99)
#define BLINK_STACK_SIZE       (64)
struct os_task blink_task;
os_stack_t blink_task_stack[BLINK_STACK_SIZE];

static volatile uint16_t timestamp;

static volatile bool fridge_on;
static bool fridge_running = false;

#define NUM_SAMPLES 8

static int16_t primary_buff[NUM_SAMPLES];
static int16_t outside_buff[NUM_SAMPLES];
static uint16_t buff_index;

static const int16_t temp_setting = (int16_t)(MYNEWT_VAL(TEMP_SETTING_C) * 100);

#define MAX_SENSORS 8
#define PRIMARY_SENSOR 0
#define OUTSIDE_SENSOR (7)

typedef struct {
  uint8_t bus;
  uint8_t addr;
  bool enabled;
  int16_t temperature;
  int16_t humidity;
} th_sensor_t;

th_sensor_t sensors[MAX_SENSORS] = {
    {0, SHT3x_ADDR,     false},
    {0, SHT3x_ALT_ADDR, false},
    {1, SHT3x_ADDR,     false},
    {1, SHT3x_ALT_ADDR, false},
    {2, SHT3x_ADDR,     false},
    {2, SHT3x_ALT_ADDR, false},
    {3, SHT3x_ADDR,     false},
    {3, SHT3x_ALT_ADDR, false},
};

#if MYNEWT_VAL(USE_BLE)
#define BEACON_MAGIC 0x0578

typedef struct {
    uint16_t magic;
    uint32_t device_id;
    uint16_t timestamp;
    uint8_t sensor_ch;
    uint8_t fridge_state;
    int16_t temperature;
    int16_t humidity;
    uint16_t flags;
} __attribute__((packed)) ble_beacon_t;

static ble_beacon_t beacon_data;

static void ble_app_advertise(bool);

static uint8_t adv_sensor_ch;

static int gap_event_cb(struct ble_gap_event *event, void *arg) {
    switch (event->type) {

    case BLE_GAP_EVENT_ADV_COMPLETE:
        ble_app_advertise(false);
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
static void ble_app_advertise(bool first_tx) {
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    if(first_tx) {
        adv_sensor_ch = 0;
        beacon_data.device_id = NRF_FICR->DEVICEADDR[0];
        beacon_data.timestamp = timestamp;
        beacon_data.fridge_state = fridge_running;
        beacon_data.flags = 0;
    }

    // Find the next enabled sensor
    while(adv_sensor_ch < MAX_SENSORS && !sensors[adv_sensor_ch].enabled) {
        adv_sensor_ch++;
    }

    if(adv_sensor_ch < MAX_SENSORS) {
        beacon_data.sensor_ch = adv_sensor_ch;
        beacon_data.temperature = sensors[adv_sensor_ch].temperature;
        beacon_data.humidity = sensors[adv_sensor_ch].humidity;

        fields = (struct ble_hs_adv_fields){ 0 };
        rc = ble_eddystone_set_adv_data_uid(&fields, &beacon_data);
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

        adv_sensor_ch++;
    }
}

static void ble_app_on_sync(void) {
    /* Generate a non-resolvable private address. */
    ble_app_set_addr();
}
#endif

void fridge_control(bool enable) {
    if(enable) {
        hal_gpio_write(FRIDGE_PIN, 1);
        hal_gpio_write(LED_2_PIN, 1);
        console_printf("Enabling fridge\n");
    } else {
        hal_gpio_write(FRIDGE_PIN, 0);
        hal_gpio_write(LED_2_PIN, 0);
        console_printf("Disabling fridge\n");
    }
}

void fridge_task_fn(void *arg) {

    hal_gpio_init_out(FRIDGE_PIN, 0);
    hal_gpio_init_out(LED_2_PIN, 0);

    while(1) {
        if(!fridge_running && fridge_on) {
            fridge_running = true;
            fridge_control(true);
        } else if(fridge_running && !fridge_on) {
            fridge_running = false;
            fridge_control(false);
            os_time_delay(MYNEWT_VAL(FRIDGE_OFF_TIME_M) * OS_TICKS_PER_SEC * 60);
        }

        os_time_delay(OS_TICKS_PER_SEC);
    }
}

void ostur_task_fn(void *arg) {
    int32_t outside_avg;
    int32_t primary_avg;

    hal_gpio_init_out(LED_1_PIN, 0);
    hal_gpio_init_out(TCA_NRST_PIN, 1);

    console_printf("Ostur Controller v2.0\n");

    i2c_init(0, SDA_PIN, SCL_PIN, 100);

    #if MYNEWT_VAL(USE_BLE)
        beacon_data.magic = BEACON_MAGIC;
        beacon_data.timestamp = 0;
    #endif

    console_printf("Detecting HT Sensors\n");
    int32_t rval;
    for(uint8_t sensor=0; sensor < MAX_SENSORS; sensor++) {
        tca95xxa_set_channel(0x70, sensors[sensor].bus);
        rval = sht3x_init(sensors[sensor].addr);
        console_printf("%d:0x%02X - %ld\n", sensors[sensor].bus, SHT3x_ADDR, rval);
        if (rval == 0) {
            sensors[sensor].enabled = true;
        }
    }

    if(sensors[OUTSIDE_SENSOR].enabled == false) {
        console_printf("Error! Outside sensor not present\n");
    }

    if(sensors[PRIMARY_SENSOR].enabled == false) {
        console_printf("Error! Primary sensor not present\n");
    }

    // TODO: Blink differently or advertise error here

    // Take a bunch of samples to initialize the buffer
    for(uint16_t sample = 0; sample < NUM_SAMPLES; sample++) {
        int16_t temp;
        tca95xxa_set_channel(0x70, sensors[OUTSIDE_SENSOR].bus);
        rval = sht3x_read(sensors[OUTSIDE_SENSOR].addr, &temp, NULL);
        outside_buff[sample] = temp;

        tca95xxa_set_channel(0x70, sensors[PRIMARY_SENSOR].bus);
        rval = sht3x_read(sensors[PRIMARY_SENSOR].addr, &temp, NULL);
        primary_buff[sample] = temp;
    }

    while (1) {
        os_time_delay(OS_TICKS_PER_SEC * MYNEWT_VAL(SAMPLE_RATE_S));

        hal_gpio_init_out(LED_1_PIN, 1);
        for(uint8_t sensor=0; sensor < MAX_SENSORS; sensor++) {
            if (!sensors[sensor].enabled) {
                continue;
            }
            int16_t *temp = &sensors[sensor].temperature;
            int16_t *humidity = &sensors[sensor].humidity;

            tca95xxa_set_channel(0x70, sensors[sensor].bus);
            rval = sht3x_read(sensors[sensor].addr, temp, humidity);
            console_printf("s: %d t:%d h:%d\n", sensor, *temp, *humidity);
        }

        // Update primary/outside buffers for average
        outside_buff[buff_index] = sensors[OUTSIDE_SENSOR].temperature;
        primary_buff[buff_index] = sensors[PRIMARY_SENSOR].temperature;
        buff_index =  (buff_index + 1) & (NUM_SAMPLES - 1);

        outside_avg = 0;
        primary_avg = 0;

        // Compute averages
        for(uint16_t index = 0; index < NUM_SAMPLES; index++){
            outside_avg += outside_buff[index];
            primary_avg += primary_buff[index];
        }

        outside_avg /= NUM_SAMPLES;
        primary_avg /= NUM_SAMPLES;

        if ((primary_avg > temp_setting) && (temp_setting < outside_avg)) {
            fridge_on = true;
        } else {
            fridge_on = false;
        }

#if MYNEWT_VAL(USE_BLE)

        ble_app_advertise(true);
#endif

        hal_gpio_init_out(LED_1_PIN, 0);

        timestamp++;
    }
}

void blink_task_fn(void *arg) {

    hal_gpio_init_out(LED_0_PIN, 0);

    while(1) {
        os_time_delay(OS_TICKS_PER_SEC);
        hal_gpio_toggle(LED_0_PIN);
    }

}

int
main(int argc, char **argv)
{
    sysinit();

    os_task_init(
        &blink_task,
        "blink_task",
        blink_task_fn,
        NULL,
        BLINK_TASK_PRI,
        OS_WAIT_FOREVER,
        blink_task_stack,
        BLINK_STACK_SIZE);

    os_task_init(
        &fridge_task,
        "fridge_task",
        fridge_task_fn,
        NULL,
        FRIDGE_TASK_PRI,
        OS_WAIT_FOREVER,
        fridge_task_stack,
        FRIDGE_STACK_SIZE);

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
