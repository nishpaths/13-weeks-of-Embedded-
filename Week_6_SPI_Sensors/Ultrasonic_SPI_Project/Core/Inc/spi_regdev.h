#ifndef SPI_REGDEV_H
#define SPI_REGDEV_H

#include "spi_bus.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * Example SPI sensor-shaped driver.
 * Shows how a real MEMS/accel driver sits on top of spi_bus.
 * WHO_AM_I style probe uses register 0x0F (ADXL345-like pattern).
 * Safe to call even with nothing on the bus — returns false on failure.
 */
typedef struct {
    spi_bus_t *bus;
    uint8_t who_am_i_reg;
    uint8_t expected_id;
} spi_regdev_t;

void spi_regdev_init(spi_regdev_t *dev, spi_bus_t *bus, uint8_t who_reg, uint8_t expected_id);
bool spi_regdev_probe(spi_regdev_t *dev);
HAL_StatusTypeDef spi_regdev_write(spi_regdev_t *dev, uint8_t reg, uint8_t value);
HAL_StatusTypeDef spi_regdev_read(spi_regdev_t *dev, uint8_t reg, uint8_t *value);

#endif /* SPI_REGDEV_H */
