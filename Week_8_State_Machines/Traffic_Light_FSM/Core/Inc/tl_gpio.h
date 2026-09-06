#ifndef TL_GPIO_H
#define TL_GPIO_H

#include "main.h"
#include <stdbool.h>

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} tl_pin_t;

typedef struct {
    tl_pin_t red;
    tl_pin_t yellow;
    tl_pin_t green;
    tl_pin_t ped;
    bool ped_present;
} tl_lamps_t;

void tl_lamps_init(tl_lamps_t *l);
void tl_lamps_all_off(tl_lamps_t *l);
void tl_lamps_set_green(tl_lamps_t *l);
void tl_lamps_set_yellow(tl_lamps_t *l);
void tl_lamps_set_red(tl_lamps_t *l);
void tl_lamps_set_red_ped(tl_lamps_t *l);

#endif /* TL_GPIO_H */
