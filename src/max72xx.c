/*
 * max72xx.c
 *
 * Minimalistic library for the MAX7219 and MAX7221 7 segment LED drivers
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 *
 */

#include "main.h"
#include "max72xx.h"

/*
 *
 * Internal functions
 *
 */


/**
 * @brief Deactivate CS (set to high)
 * @param Pointer to a max72xx handle
 */
void cs_off(MAX72XX_HandleTypeDef *max72xx) {
	HAL_GPIO_WritePin(max72xx->cs_port, max72xx->cs_pin, GPIO_PIN_SET);
}

/**
 * @brief Activate CS (set to high)
 * @param Pointer to a max72xx handle
 */
void cs_on(MAX72XX_HandleTypeDef *max72xx) {
	HAL_GPIO_WritePin(max72xx->cs_port, max72xx->cs_pin, GPIO_PIN_RESET);
}

MAX72XX_result_t max72xx_transmit(MAX72XX_HandleTypeDef *max72xx, uint8_t *buf, uint32_t len) {
	MAX72XX_result_t result = MAX72XX_Ok;

	cs_on(max72xx);
	if (HAL_SPI_Transmit(max72xx->spiHandle, buf, len, HAL_MAX_DELAY) != HAL_OK) {
		result = MAX72XX_Err;
	}
	cs_off(max72xx);

	return result;
}

/*
 *
 * Public Functions
 *
 */

/**
 * @brief Initialize the MAX72XX library/driver.
 * @param Pointer to a max72xx handle
 * @param hspi pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.
 * @param Chip select port
 * @param Chip select pin
 * @retval MAX72XX Status
 */
MAX72XX_result_t max72xx_init(MAX72XX_HandleTypeDef *max72xx, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin) {

	MAX72XX_result_t result = MAX72XX_Ok;

	max72xx->spiHandle = hspi;
	max72xx->cs_port = cs_port;
	max72xx->cs_pin = cs_pin;

	cs_off(max72xx);

	return result;

}
