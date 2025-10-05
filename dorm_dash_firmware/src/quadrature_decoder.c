#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "quadrature_decoder.h"

LOG_MODULE_REGISTER(quadrature_decoder);

#define ENC_NODE_0 DT_NODELABEL(qenc0)
#define ENC_NODE_1 DT_NODELABEL(qenc1)

static const struct gpio_dt_spec enc0_a_spec = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_0, gpios, 0);
static const struct gpio_dt_spec enc0_b_spec = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_0, gpios, 1);

static const struct gpio_dt_spec enc1_a_spec = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_1, gpios, 0);
static const struct gpio_dt_spec enc1_b_spec = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_1, gpios, 1);

static struct gpio_callback enc0_a_cb_data;
static struct gpio_callback enc0_b_cb_data;

static struct gpio_callback enc1_a_cb_data;
static struct gpio_callback enc1_b_cb_data;


int enc0_dir = 0;
static atomic_t enc0_pos = ATOMIC_INIT(0);

int enc1_dir = 0;
static atomic_t enc1_pos = ATOMIC_INIT(0);

static long get_enc0_pos()
{
    return atomic_get(&enc0_pos);
}

static long get_enc1_pos()
{
    return atomic_get(&enc1_pos);
}

static void enc0_a_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	if (gpio_pin_get_dt(&enc0_b_spec) == 1)
	{
	    enc0_dir = 1;
	}

    // Increment in current direction
    atomic_val_t val = atomic_get(&enc0_pos);
    atomic_set(&enc0_pos, val + enc0_dir);
}

static void enc0_b_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if (gpio_pin_get_dt(&enc0_a_spec) == 1)
    {
        enc0_dir = -1;
    }

    // Increment in current direction
    atomic_val_t val = atomic_get(&enc0_pos);
    atomic_set(&enc0_pos, val + enc0_dir);
}

static void enc1_a_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if (gpio_pin_get_dt(&enc1_b_spec) == 1)
    {
        enc1_dir = 1;
    }

    // Increment in current direction
    atomic_val_t val = atomic_get(&enc1_pos);
    atomic_set(&enc1_pos, val + enc1_dir);
}

static void enc1_b_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if (gpio_pin_get_dt(&enc1_a_spec) == 1)
    {
        enc1_dir = -1;
    }

    // Increment in current direction
    atomic_val_t val = atomic_get(&enc1_pos);
    atomic_set(&enc1_pos, val + enc1_dir);
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

    if (!gpio_is_ready_dt(&enc1_a_spec))
    {
        LOG_ERR("Encoder1_A gpio is not ready!");
        return -1;
    }

    if (!gpio_is_ready_dt(&enc1_b_spec))
    {
        LOG_ERR("Encoder1_B gpio is not ready!");
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

    if (gpio_pin_configure_dt(&enc1_a_spec, GPIO_INPUT) < 0)
    {
        LOG_ERR("Unable to configure Encoder1_A gpio!");
        return -1;
    }

    if (gpio_pin_configure_dt(&enc1_b_spec, GPIO_INPUT) < 0)
    {
        LOG_ERR("Unable to configure Encoder1_B gpio!");
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

    ret = gpio_pin_interrupt_configure_dt(&enc1_a_spec, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        LOG_ERR("Failed to configure Encoder1_A interrupt!");
        return -1;
    }

    ret = gpio_pin_interrupt_configure_dt(&enc1_b_spec, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        LOG_ERR("Failed to configure Encoder1_B interrupt!");
        return -1;
    }

    gpio_init_callback(&enc0_a_cb_data, enc0_a_callback, BIT(enc0_a_spec.pin));
    gpio_init_callback(&enc0_b_cb_data, enc0_b_callback, BIT(enc0_b_spec.pin));

    gpio_add_callback(enc0_a_spec.port, &enc0_a_cb_data);
    gpio_add_callback(enc0_b_spec.port, &enc0_b_cb_data);

    gpio_init_callback(&enc1_a_cb_data, enc1_a_callback, BIT(enc1_a_spec.pin));
    gpio_init_callback(&enc1_b_cb_data, enc1_b_callback, BIT(enc1_b_spec.pin));

    gpio_add_callback(enc1_a_spec.port, &enc1_a_cb_data);
    gpio_add_callback(enc1_b_spec.port, &enc1_b_cb_data);


    while (true)
    {
        long enc0_pos = get_enc0_pos();
        long enc1_pos = get_enc1_pos();

        LOG_INF("enc0: %ld | enc1: %ld", enc0_pos, enc1_pos);
        k_sleep(K_MSEC(10));
    }

    return 0;
}
