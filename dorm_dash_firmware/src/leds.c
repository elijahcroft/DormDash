#include "leds.h"

#define THREAD_PRIO 10
#define THREAD_STACK_SIZE 512
static K_THREAD_STACK_DEFINE(stack_area, THREAD_STACK_SIZE);
static struct k_thread thread_data;

#define SLEEP_TIME_MS 2000
#define LED0_NODE DT_ALIAS(led0)

LOG_MODULE_REGISTER(leds);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);


static void leds_thread_entry(void *a, void *b, void *c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    bool led_state = true;

    while (true) {

        if (gpio_pin_toggle_dt(&led) < 0) {
            LOG_ERR("Could not toggle LED GPIO!");
            return;
        }

        led_state = !led_state;
        k_msleep(SLEEP_TIME_MS);
    }
}

int LEDS_Init()
{

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO not ready!");
        return -1;
    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
        LOG_ERR("Could not set LED GPIO to HIGH");
        return -1;
    }

    k_thread_create(&thread_data, stack_area, K_THREAD_STACK_SIZEOF(stack_area), leds_thread_entry, NULL,
                    NULL, NULL, THREAD_PRIO, 0, K_NO_WAIT);

    return 0;
}
