

#ifndef TIMER_H
#define TIMER_H

#include "main.h"
#include <stdbool.h>

typedef struct{
    TIM_HandleTypeDef *handle;
    volatile bool event_occurred;
}timer_t;

void timer_init(timer_t *timer, TIM_HandleTypeDef *handle);
void timer_start(timer_t *timer);
void timer_stop(timer_t *timer);
bool timer_event_occurred(timer_t *timer);
void timer_set_event(timer_t *timer);


#endif /* __TIMER_H__ */
