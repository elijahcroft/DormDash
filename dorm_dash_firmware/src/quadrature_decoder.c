#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "quadrature_decoder.h"
#include <zephyr/sys/hash_map.h>


LOG_MODULE_REGISTER(quadrature_decoder);

#define ENC_NODE_0 DT_NODELABEL(qenc0)
#define ENC_NODE_1 DT_NODELABEL(qenc1)

struct quadrature_encoder {
    const struct gpio_dt_spec chan_a;
    const struct gpio_dt_spec chan_b;
    struct gpio_callback chan_a_callback_data;
    struct gpio_callback chan_b_callback_data;
    gpio_callback_handler_t interrupt_callback;

    int direction;
    atomic_t position;
    char* name;
};

#define QUADRATURE_ENC_COUNT 2

void qenc0_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void qenc1_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins);


struct quadrature_encoder quad_encoders[QUADRATURE_ENC_COUNT] = {
    {
        .name = "enc0",
		.chan_a = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_0, gpios, 0),
        .chan_b = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_0, gpios, 1),
        .interrupt_callback = qenc0_callback,

        .direction = 0,
        .position = ATOMIC_INIT(0),
	},

    {
        .name = "enc1",
        .chan_a = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_1, gpios, 0),
        .chan_b = GPIO_DT_SPEC_GET_BY_IDX(ENC_NODE_1, gpios, 1),
        .interrupt_callback = qenc1_callback,

        .direction = 0,
        .position = ATOMIC_INIT(0)
    },

};

long QuadratureEncoder_GetPos(enum QuadEncoderType type)
{
    return atomic_get(&quad_encoders[type].position);
}

void qenc_update_position(struct quadrature_encoder* qenc);

void qenc0_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    qenc_update_position(&quad_encoders[Q_ENCODER_LEFT_MOTOR]);
}

void qenc1_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    qenc_update_position(&quad_encoders[Q_ENCODER_RIGHT_MOTOR]);
}

void qenc_update_position(struct quadrature_encoder* qenc)
{
    int chan_a = gpio_pin_get_dt(&qenc->chan_a);
    int chan_b = gpio_pin_get_dt(&qenc->chan_b);

    if (chan_a == 1 && chan_b == 0)
    {
        qenc->direction = 1;
    }
    else if (chan_a == 0 && chan_b == 1)
    {
        qenc->direction = -1;
    }
    else
    {
        // Unknown state
        return;
    }

    // Increment in current direction
    atomic_val_t val = atomic_get(&qenc->position);
    atomic_set(&qenc->position, val + qenc->direction);
}

bool check_all_devices_ready(void)
{
    bool all_devices_ready = true;

    for (int i = 0; i < QUADRATURE_ENC_COUNT; i++)
    {
        if (!gpio_is_ready_dt(&quad_encoders[i].chan_a) || !gpio_is_ready_dt(&quad_encoders[i].chan_a))
        {
            all_devices_ready = false;
            LOG_ERR("%s gpio channel A or B is not ready!", quad_encoders[i].name);
        }
    }

    return all_devices_ready;
}

int configure_all_gpios()
{
    int ret = 0;

    for (int i = 0; i < QUADRATURE_ENC_COUNT; i++)
    {
        if (gpio_pin_configure_dt(&quad_encoders[i].chan_a, GPIO_INPUT) < 0
            || gpio_pin_configure_dt(&quad_encoders[i].chan_b, GPIO_INPUT) < 0)
        {
            LOG_ERR("Unable to configure GPIO for %s!", quad_encoders[i].name);
            ret = -1;
        }
    }

    return ret;
}

int configure_all_gpio_interrupts()
{
    for (int i = 0; i < QUADRATURE_ENC_COUNT; i++)
    {
        if (gpio_pin_interrupt_configure_dt(&quad_encoders[i].chan_a, GPIO_INT_EDGE_TO_ACTIVE) < 0
            || gpio_pin_interrupt_configure_dt(&quad_encoders[i].chan_b, GPIO_INT_EDGE_TO_ACTIVE) < 0)
        {
            LOG_ERR("Unable to configure gpio for interrupt: %s", quad_encoders[i].name);
            return -1;
        }

        gpio_init_callback(&quad_encoders[i].chan_a_callback_data, quad_encoders[i].interrupt_callback, BIT(quad_encoders[i].chan_a.pin));
        gpio_init_callback(&quad_encoders[i].chan_b_callback_data, quad_encoders[i].interrupt_callback, BIT(quad_encoders[i].chan_b.pin));

        if (gpio_add_callback(quad_encoders[i].chan_a.port, &quad_encoders[i].chan_a_callback_data) < 0
            || gpio_add_callback(quad_encoders[i].chan_b.port, &quad_encoders[i].chan_b_callback_data) < 0)
        {
            LOG_ERR("Unable to add interrupt callbacks for quad encoder: %s", quad_encoders[i].name);
            return -1;
        }
    }


    return 0;
}

int QuadratureDecoder_Start(void)
{
    if (!check_all_devices_ready())
    {
        LOG_ERR("Not all quadrature encoder GPIO ready!");
        return -1;
    }

    if (configure_all_gpios() < 0)
    {
        LOG_ERR("Unable to configure qenc GPIOS.");
        return -1;
    }

    if (configure_all_gpio_interrupts() < 0)
    {
        LOG_ERR("Unable to configure all qenc interrupts or callbacks");
        return -1;
    }

	return 0;

    //while (true)
    //{
    //    long left_pos = QuadratureEncoder_GetPos(Q_ENCODER_LEFT_MOTOR);
    //    long right_pos = QuadratureEncoder_GetPos(Q_ENCODER_RIGHT_MOTOR);

		//LOG_INF("callback0: %p", &quad_encoders[0].interrupt_callback);
		//LOG_INF("callback1: %p", &quad_encoders[1].interrupt_callback);

   //     LOG_INF("left_motor__: %ld | right_motor: %ld", left_pos, right_pos);
    //    k_sleep(K_MSEC(10));
    //}

    //return 0;
}
