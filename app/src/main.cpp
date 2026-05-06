#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* The devicetree node identifier for the "led0" alias. */
#define HEARTBEAT_LED DT_ALIAS(app_led)

static const struct gpio_dt_spec heartbeat_led = GPIO_DT_SPEC_GET(HEARTBEAT_LED, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool heartbeat_led_state = true;

    if (!gpio_is_ready_dt(&heartbeat_led)) return 0;

    if (gpio_pin_configure_dt(&heartbeat_led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&heartbeat_led) < 0) return 0;

        heartbeat_led_state = !heartbeat_led_state;
        LOG_INF("Heartbeat LED state: %s", heartbeat_led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
