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
#define THREAD_STACK_SIZE 8192
static K_THREAD_STACK_DEFINE(stack_area, THREAD_STACK_SIZE);
static struct k_thread thread_data;

LOG_MODULE_REGISTER(distance_sensor);

#define LIDAR_NODE_0 DT_NODELABEL(lidar0)
#define LIDAR_NODE_1 DT_NODELABEL(lidar1)
#define LIDAR_NODE_2 DT_NODELABEL(lidar2)
#define LIDAR_NODE_3 DT_NODELABEL(lidar3)
//#define LIDAR_NODE_4 DT_NODELABEL(lidar4)

#define LIDAR_DEVICE_COUNT 4

const struct device *const devices[LIDAR_DEVICE_COUNT] = {
	DEVICE_DT_GET(LIDAR_NODE_0),
	DEVICE_DT_GET(LIDAR_NODE_1),
	DEVICE_DT_GET(LIDAR_NODE_2),
	DEVICE_DT_GET(LIDAR_NODE_3),
	//DEVICE_DT_GET(LIDAR_NODE_4),
};

static bool check_all_devices_ready() {
	bool all_devices_ready = true;

	for (int i = 0; i < LIDAR_DEVICE_COUNT; i++) {
		if (!device_is_ready(devices[i])) {
			all_devices_ready = false;
			LOG_ERR("Device: %s is not ready!", devices[i]->name);
		}
	}

	return all_devices_ready;
}

static void fetch_all_device_samples() {
	for (int i = 0; i < LIDAR_DEVICE_COUNT; i++) {

		int ret = sensor_sample_fetch_chan(devices[i], SENSOR_CHAN_DISTANCE);
		if (ret) {
			LOG_ERR("%s: failed to fetch sensor data, ret %d\n", devices[i]->name, ret);
		}
	}
}

static void lidar_thread_entry(void* a, void* b, void* c)
{
	ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    //int ret;
    struct sensor_value value;

	while (true) {

		fetch_all_device_samples();

		for (int i = 0; i < LIDAR_DEVICE_COUNT; i++) {

			int ret = sensor_channel_get(devices[i], SENSOR_CHAN_DISTANCE, &value);
			if (ret < 0) {
				LOG_ERR("%s, Could not GET sensor data! Ret: %d\n", devices[i]->name, ret);
			} else {
				LOG_INF("%s distance: %.3lld mm", devices[i]->name, sensor_value_to_milli(&value));
			}
		}

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
	if (!check_all_devices_ready()) {
		LOG_ERR("Not all LiDAR devices ready!");
		return -1;
	}

    k_thread_create(&thread_data, stack_area, K_THREAD_STACK_SIZEOF(stack_area), lidar_thread_entry, NULL,
                    NULL, NULL, THREAD_PRIO, 0, K_NO_WAIT);

    return 0;
}
