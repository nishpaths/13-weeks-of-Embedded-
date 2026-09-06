#include "tl_gpio.h"

static void write_pin(tl_pin_t p, GPIO_PinState s)
{
    HAL_GPIO_WritePin(p.port, p.pin, s);
}

void tl_lamps_init(tl_lamps_t *l)
{
    tl_lamps_all_off(l);
}

void tl_lamps_all_off(tl_lamps_t *l)
{
    write_pin(l->red, GPIO_PIN_RESET);
    write_pin(l->yellow, GPIO_PIN_RESET);
    write_pin(l->green, GPIO_PIN_RESET);
    if (l->ped_present) {
        write_pin(l->ped, GPIO_PIN_RESET);
    }
}

void tl_lamps_set_green(tl_lamps_t *l)
{
    write_pin(l->red, GPIO_PIN_RESET);
    write_pin(l->yellow, GPIO_PIN_RESET);
    write_pin(l->green, GPIO_PIN_SET);
    if (l->ped_present) {
        write_pin(l->ped, GPIO_PIN_RESET);
    }
}

void tl_lamps_set_yellow(tl_lamps_t *l)
{
    write_pin(l->red, GPIO_PIN_RESET);
    write_pin(l->yellow, GPIO_PIN_SET);
    write_pin(l->green, GPIO_PIN_RESET);
    if (l->ped_present) {
        write_pin(l->ped, GPIO_PIN_RESET);
    }
}

void tl_lamps_set_red(tl_lamps_t *l)
{
    write_pin(l->red, GPIO_PIN_SET);
    write_pin(l->yellow, GPIO_PIN_RESET);
    write_pin(l->green, GPIO_PIN_RESET);
    if (l->ped_present) {
        write_pin(l->ped, GPIO_PIN_RESET);
    }
}

void tl_lamps_set_red_ped(tl_lamps_t *l)
{
    write_pin(l->red, GPIO_PIN_SET);
    write_pin(l->yellow, GPIO_PIN_RESET);
    write_pin(l->green, GPIO_PIN_RESET);
    if (l->ped_present) {
        write_pin(l->ped, GPIO_PIN_SET);
    }
}
