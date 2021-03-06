/*
 *  TOTOLINK A3002RU board support
 *
 *  Copyright (C) 2018 Dimazhan <dimazhan@list.ru>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach-realtek/realtek.h>
#include <asm/mach-realtek/platform.h>

#include "machtypes.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-eth.h"
#include "gpio.h"

#define TL_GPIO_LED_USB				4 // GPIO pin, A4 !-
#define TL_GPIO_LED_CPU				6 // GPIO pin, A6 !WPS

#define TL_GPIO_BTN_WPS				3 // GPIO pin, A3 !+
#define TL_GPIO_BTN_RESET			5 // GPIO pin, A5 !+

#define TL_KEYS_POLL_INTERVAL		100	/* msecs */
#define TL_KEYS_DEBOUNCE_INTERVAL	(3 * TL_KEYS_POLL_INTERVAL)

static struct gpio_led tl_leds_gpio[] __initdata = {
	{
		.name		= "tl:orange:usb",
		.gpio		= TL_GPIO_LED_USB,
		.active_low	= 1,
	}, {
		.name		= "tl:orange:cpu",
		.gpio		= TL_GPIO_LED_CPU,
		.active_low	= 1,
	},
};

static struct gpio_keys_button tl_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = TL_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TL_GPIO_BTN_RESET,
		.active_low	= 1,
	} , {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = TL_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TL_GPIO_BTN_WPS,
		.active_low	= 1,
	}
};

static void __init a3002ru_init(void)
{
	realtek_register_leds_gpio(-1, ARRAY_SIZE(tl_leds_gpio),
				 tl_leds_gpio);
	realtek_register_gpio_keys_polled(-1, TL_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(tl_gpio_keys),
					tl_gpio_keys);

	realtek_register_m25p80(NULL);
	//realtek_register_eth();
	//def mac 56aaa55a7de8

	realtek_set_gpio_control(TL_GPIO_LED_USB, true);
	realtek_set_gpio_control(TL_GPIO_LED_CPU, true);
	realtek_set_gpio_control(TL_GPIO_BTN_WPS, true);
	realtek_set_gpio_control(TL_GPIO_BTN_RESET, true);

	realtek_set_gpio_mux(
		RTL819XD_GPIO_MUX_UART0 | RTL819XD_GPIO_MUX_PCIE_RST |
		(RTL819XD_GPIO_MUX_JTAG_MASK_CLEAR << RTL819XD_GPIO_MUX_JTAG_SHIFT) |
		(RTL819XD_GPIO_MUX_P0MDIO_MASK<<RTL819XD_GPIO_MUX_P0MDIO_SHIFT),
		(RTL819XD_GPIO_MUX_JTAG_MASK_SET << RTL819XD_GPIO_MUX_JTAG_SHIFT) |
		(RTL819XD_GPIO_MUX_FCS1N_MASK << RTL819XD_GPIO_MUX_FCS1N_SHIFT));
}

MIPS_MACHINE(REALTEK_MACH_A3002RU, "A3002RU", "TOTOLINK A3002RU",
	     a3002ru_init);
