/*
 * Copyright (c) 2017 STMicroelectronics
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/vl53l0x.h>
#include <zephyr/logging/log.h>

#define THREAD_PRIO 10
#define THREAD_STACK_SIZE 512
static K_THREAD_STACK_DEFINE(stack_area, THREAD_STACK_SIZE);
static struct k_thread thread_data;

LOG_MODULE_REGISTER(distance_sensor);

#define TRIAL_N 0

#if TRIAL_N == 0
    #define LIDAR_NODE DT_PATH(soc, i2c_40044000, lidar_29)
#elif TRIAL_N == 1
    #define LIDAR_NODE DT_NODELABEL(lidar0)
#else
    #define LIDAR_NODE DT_CHILD(DT_NODELABEL(i2c0), lidar_29)
#endif

static const struct device *const dev = DEVICE_DT_GET(LIDAR_NODE);

static void lidar_thread_entry(void* a, void* b, void* c)
{
	ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    //int ret;
    struct sensor_value value;

	while (true) {

		int ret = sensor_sample_fetch(dev);

		if (ret) {
			LOG_INF("sensor_sample_fetch failed ret %d\n", ret);
			return;
		}

		// ret = sensor_channel_get(dev, SENSOR_CHAN_PROX, &value);
		// LOG_INF("prox is %d\n", value.val1);

		ret = sensor_channel_get(dev, SENSOR_CHAN_DISTANCE, &value);
		LOG_INF("distance is %.3lld mm", sensor_value_to_milli(&value));

		// ret = sensor_channel_get(dev, SENSOR_CHAN_VL53L0X_RANGE_DMAX, &value);
		// LOG_INF("Max distance is %.3lld mm\n", sensor_value_to_milli(&value));

		// ret = sensor_channel_get(dev, SENSOR_CHAN_VL53L0X_SIGNAL_RATE_RTN_CPS, &value);
		// LOG_INF("Signal rate is %d Cps\n", value.val1);

		// ret = sensor_channel_get(dev, SENSOR_CHAN_VL53L0X_AMBIENT_RATE_RTN_CPS, &value);
		// LOG_INF("Ambient rate is %d Cps\n", value.val1);

		// ret = sensor_channel_get(dev, SENSOR_CHAN_VL53L0X_EFFECTIVE_SPAD_RTN_COUNT, &value);
		// LOG_INF("SPADs used: %d\n", value.val1);

		// ret = sensor_channel_get(dev, SENSOR_CHAN_VL53L0X_RANGE_STATUS, &value);
		// if (value.val1 == VL53L0X_RANGE_STATUS_RANGE_VALID) {
		// 	LOG_INF("Status: OK\n");
		// } else {
		// 	LOG_INF("Status: Error code %d\n", value.val1);
		// }

		// LOG_INF("\n");
		k_sleep(K_MSEC(100));
	}
	return;
}

int LIDAR_Start(void)
{
	//struct sensor_value value;
	//int ret;

	if (!device_is_ready(dev)) {
		LOG_ERR("LiDAR not ready!");
		return -1;
	}

    k_thread_create(&thread_data, stack_area, K_THREAD_STACK_SIZEOF(stack_area), lidar_thread_entry, NULL,
                    NULL, NULL, THREAD_PRIO, 0, K_NO_WAIT);

    return 0;
}
