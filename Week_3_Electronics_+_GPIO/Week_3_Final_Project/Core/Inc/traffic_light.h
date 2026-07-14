#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_
#include <stdint.h>
#include "main.h"

void traffic_light_init(void);

void set_green(void);
void set_yellow(void);
void set_red(void);
void traffic_light_update(uint32_t now);

#endif /* INC_TRAFFIC_LIGHT_H_ */