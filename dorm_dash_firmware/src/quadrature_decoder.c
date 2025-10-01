#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "quadrature_decoder.h"

LOG_MODULE_REGISTER(quadrature_decoder);

#define ENC_NODE_0 DT_NODELABEL(qenc0)
// #define ENC_NODE_1 DT_NODELABEL(qenc1)

static const struct gpio_dt_spec enc0_a_spec = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_0, gpios, 0);
static const struct gpio_dt_spec enc0_b_spec = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_0, gpios, 1);

static struct gpio_callback enc0_a_cb_data;
static struct gpio_callback enc0_b_cb_data;

static int enc0_a_edge_count = 0;
static int enc0_b_edge_count = 0;

// clockwise = BA
// c-clockwiese = AB

int dir = 0;

int current_set = 0;
int last_set = 0;

static void enc0_a_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    current_set = -1;

    if (current_set == -1 && last_set == 1)
    {
        dir = -1;
        LOG_INF("DIR: %d", dir);

    }

    last_set = current_set;
    // enc0_a_edge_count++;
    // LOG_INF("A Edge: %d", enc0_a_edge_count);
    //
    // if (enc0_a_edge_count > enc0_b_edge_count)
    // {
    //     enc0_a_edge_count = 0;
    //     enc0_b_edge_count = 0;
    //
    //     dir = 1;
    //     LOG_INF("Direction %d", dir);
    // }
}

static void enc0_b_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    current_set = 1;

    if (current_set == 1 && last_set == -1)
    {
        dir = 1;
        LOG_INF("DIR: %d", dir);
    }

    last_set = current_set;
    // enc0_b_edge_count++;
    // LOG_INF("B Edge: %d", enc0_b_edge_count);
    // if (enc0_b_edge_count > enc0_a_edge_count)
    // {
    //     enc0_b_edge_count = 0;
    //     enc0_a_edge_count = 0;
    //
    //     dir = -1;
    //     LOG_INF("Direction %d", dir);
    // }
}

int QuadratureDecoder_Start(void)
{
    if (!gpio_is_ready_dt(&enc0_a_spec))
    {
        LOG_ERR("Encoder0_A gpio is not ready!");
        return -1;
    }

    if (!gpio_is_ready_dt(&enc0_b_spec))
    {
        LOG_ERR("Encoder0_B gpio is not ready!");
        return -1;
    }

    if (gpio_pin_configure_dt(&enc0_a_spec, GPIO_INPUT) < 0)
    {
        LOG_ERR("Unable to configure Encoder0_A gpio!");
        return -1;
    }

    if (gpio_pin_configure_dt(&enc0_b_spec, GPIO_INPUT) < 0)
    {
        LOG_ERR("Unable to configure Encoder0_B gpio!");
        return -1;
    }

    int ret;

    ret = gpio_pin_interrupt_configure_dt(&enc0_a_spec, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        LOG_ERR("Failed to configure Encoder0_A interrupt!");
        return -1;
    }

    ret = gpio_pin_interrupt_configure_dt(&enc0_b_spec, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        LOG_ERR("Failed to configure Encoder0_B interrupt!");
        return -1;
    }

    gpio_init_callback(&enc0_a_cb_data, enc0_a_callback, BIT(enc0_a_spec.pin));
    gpio_init_callback(&enc0_b_cb_data, enc0_b_callback, BIT(enc0_b_spec.pin));

    gpio_add_callback(enc0_a_spec.port, &enc0_a_cb_data);
    gpio_add_callback(enc0_b_spec.port, &enc0_b_cb_data);

    return 0;
}
