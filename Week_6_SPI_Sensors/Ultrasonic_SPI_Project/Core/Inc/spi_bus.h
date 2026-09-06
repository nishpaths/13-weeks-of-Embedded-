#ifndef SPI_BUS_H
#define SPI_BUS_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * Thin SPI master wrapper.
 * Sensor drivers depend on this — not on HAL SPI calls directly.
 */
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    uint32_t timeout_ms;
} spi_bus_t;

void spi_bus_init(spi_bus_t *bus,
                  SPI_HandleTypeDef *hspi,
                  GPIO_TypeDef *cs_port,
                  uint16_t cs_pin,
                  uint32_t timeout_ms);

void spi_bus_cs_select(spi_bus_t *bus);
void spi_bus_cs_deselect(spi_bus_t *bus);

HAL_StatusTypeDef spi_bus_transmit(spi_bus_t *bus, const uint8_t *tx, uint16_t len);
HAL_StatusTypeDef spi_bus_receive(spi_bus_t *bus, uint8_t *rx, uint16_t len);
HAL_StatusTypeDef spi_bus_transfer(spi_bus_t *bus, const uint8_t *tx, uint8_t *rx, uint16_t len);

/** Write one register: [addr | 0x00] [value] with CS framing. */
HAL_StatusTypeDef spi_bus_write_reg(spi_bus_t *bus, uint8_t reg, uint8_t value);

/** Read one register: [addr | 0x80] then clock in one byte (common MEMS pattern). */
HAL_StatusTypeDef spi_bus_read_reg(spi_bus_t *bus, uint8_t reg, uint8_t *value);

#endif /* SPI_BUS_H */
