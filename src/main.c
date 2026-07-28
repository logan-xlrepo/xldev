/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>

/* 板载 LED（如果 devicetree 中有定义） */
#if DT_NODE_EXISTS(DT_ALIAS(led0))
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
#endif

int main(void)
{
	int ret;

	printf("xlfirmware starting...\n");
	printf("Board: %s\n", CONFIG_BOARD);

#if DT_NODE_EXISTS(DT_ALIAS(led0))
	if (!device_is_ready(led.port)) {
		printf("Error: LED device not ready\n");
		return 1;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printf("Error: failed to configure LED pin (%d)\n", ret);
		return 1;
	}
	printf("LED configured, starting blink...\n");
#else
	printf("No LED defined in devicetree, running in simulation mode...\n");
#endif

	while (1) {
#if DT_NODE_EXISTS(DT_ALIAS(led0))
		gpio_pin_toggle_dt(&led);
		printf("LED %s\n", gpio_pin_get_dt(&led) ? "ON" : "OFF");
#else
		printf("Tick...%d...\n", ret++);
#endif
		k_sleep(K_MSEC(1000));
	}

	return 0;
}
