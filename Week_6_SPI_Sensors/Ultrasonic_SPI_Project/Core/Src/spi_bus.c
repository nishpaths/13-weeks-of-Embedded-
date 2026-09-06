#include "spi_bus.h"

void spi_bus_init(spi_bus_t *bus,
                  SPI_HandleTypeDef *hspi,
                  GPIO_TypeDef *cs_port,
                  uint16_t cs_pin,
                  uint32_t timeout_ms)
{
    bus->hspi = hspi;
    bus->cs_port = cs_port;
    bus->cs_pin = cs_pin;
    bus->timeout_ms = timeout_ms;
    spi_bus_cs_deselect(bus);
}

void spi_bus_cs_select(spi_bus_t *bus)
{
    HAL_GPIO_WritePin(bus->cs_port, bus->cs_pin, GPIO_PIN_RESET);
}

void spi_bus_cs_deselect(spi_bus_t *bus)
{
    HAL_GPIO_WritePin(bus->cs_port, bus->cs_pin, GPIO_PIN_SET);
}

HAL_StatusTypeDef spi_bus_transmit(spi_bus_t *bus, const uint8_t *tx, uint16_t len)
{
    if (bus == NULL || bus->hspi == NULL || tx == NULL) {
        return HAL_ERROR;
    }
    return HAL_SPI_Transmit(bus->hspi, (uint8_t *)tx, len, bus->timeout_ms);
}

HAL_StatusTypeDef spi_bus_receive(spi_bus_t *bus, uint8_t *rx, uint16_t len)
{
    if (bus == NULL || bus->hspi == NULL || rx == NULL) {
        return HAL_ERROR;
    }
    return HAL_SPI_Receive(bus->hspi, rx, len, bus->timeout_ms);
}

HAL_StatusTypeDef spi_bus_transfer(spi_bus_t *bus, const uint8_t *tx, uint8_t *rx, uint16_t len)
{
    if (bus == NULL || bus->hspi == NULL || tx == NULL || rx == NULL) {
        return HAL_ERROR;
    }
    return HAL_SPI_TransmitReceive(bus->hspi, (uint8_t *)tx, rx, len, bus->timeout_ms);
}

HAL_StatusTypeDef spi_bus_write_reg(spi_bus_t *bus, uint8_t reg, uint8_t value)
{
    uint8_t frame[2] = { (uint8_t)(reg & 0x7FU), value };
    HAL_StatusTypeDef st;

    spi_bus_cs_select(bus);
    st = spi_bus_transmit(bus, frame, 2);
    spi_bus_cs_deselect(bus);
    return st;
}

HAL_StatusTypeDef spi_bus_read_reg(spi_bus_t *bus, uint8_t reg, uint8_t *value)
{
    uint8_t tx[2] = { (uint8_t)(reg | 0x80U), 0x00U };
    uint8_t rx[2] = { 0 };
    HAL_StatusTypeDef st;

    if (value == NULL) {
        return HAL_ERROR;
    }

    spi_bus_cs_select(bus);
    st = spi_bus_transfer(bus, tx, rx, 2);
    spi_bus_cs_deselect(bus);

    if (st == HAL_OK) {
        *value = rx[1];
    }
    return st;
}
