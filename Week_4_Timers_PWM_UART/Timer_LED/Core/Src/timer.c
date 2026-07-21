#include "timer.h"


void timer_init(timer_t *timer, TIM_HandleTypeDef *handle){
    timer->handle = handle;
    timer->event_occurred = false;
}
void timer_start(timer_t *timer){
    HAL_TIM_Base_Start_IT(timer->handle);
}
void timer_stop(timer_t *timer){
    HAL_TIM_Base_Stop_IT(timer->handle);
}
bool timer_event_occurred(timer_t *timer){
    if(timer->event_occurred){
        timer->event_occurred = false;
        return true;
    }
    return false;
}
void timer_set_event(timer_t *timer){
    timer->event_occurred = true;
}
