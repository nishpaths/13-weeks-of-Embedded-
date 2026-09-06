#include "spi_regdev.h"

void spi_regdev_init(spi_regdev_t *dev, spi_bus_t *bus, uint8_t who_reg, uint8_t expected_id)
{
    dev->bus = bus;
    dev->who_am_i_reg = who_reg;
    dev->expected_id = expected_id;
}

bool spi_regdev_probe(spi_regdev_t *dev)
{
    uint8_t id = 0;
    if (spi_bus_read_reg(dev->bus, dev->who_am_i_reg, &id) != HAL_OK) {
        return false;
    }
    return id == dev->expected_id;
}

HAL_StatusTypeDef spi_regdev_write(spi_regdev_t *dev, uint8_t reg, uint8_t value)
{
    return spi_bus_write_reg(dev->bus, reg, value);
}

HAL_StatusTypeDef spi_regdev_read(spi_regdev_t *dev, uint8_t reg, uint8_t *value)
{
    return spi_bus_read_reg(dev->bus, reg, value);
}
