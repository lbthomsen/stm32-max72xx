/*
 * max72xx.c
 *
 * Minimalistic library for the MAX7219 and MAX7221 7 segment LED drivers
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 *
 */

#include <math.h>
#include "main.h"
#include "max72xx.h"

/*
 *
 * Private Internal Functions
 *
 */

void cs_enable(MAX72XX_HandleTypeDef *max72xx) {
	HAL_GPIO_WritePin(max72xx->cs_port, max72xx->cs_pin, GPIO_PIN_RESET);
}

void cs_disable(MAX72XX_HandleTypeDef *max72xx) {
	HAL_GPIO_WritePin(max72xx->cs_port, max72xx->cs_pin, GPIO_PIN_SET);
}

MAX72XX_result_t max72xx_transmit(MAX72XX_HandleTypeDef *max72xx, uint8_t address, uint8_t value) {
	MAX72XX_result_t result = MAX72XX_Ok;

	//uint16_t data = (address << 8) + value;

	cs_enable(max72xx);
	HAL_SPI_Transmit(max72xx->spiHandle, (uint8_t *)&address, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(max72xx->spiHandle, (uint8_t *)&value, 1, HAL_MAX_DELAY);
	cs_disable(max72xx);

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
MAX72XX_result_t max72xx_init(MAX72XX_HandleTypeDef *max72xx, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t digits) {

	MAX72XX_result_t result = MAX72XX_Ok;

	max72xx->spiHandle = hspi;
	max72xx->cs_port = cs_port;
	max72xx->cs_pin = cs_pin;
	max72xx->digits = digits;


	max72xx_transmit(max72xx, MAX72XX_SHUTDOWN, 0x01);
	max72xx_transmit(max72xx, MAX72XX_DISPLAY_TEST, 0x00);
	max72xx_transmit(max72xx, MAX72XX_SCAN_LIMIT, digits - 1);
	max72xx_transmit(max72xx, MAX72XX_INTENSITY, 0x05);

	max72xx_transmit(max72xx, MAX72XX_DECODE_MODE, 0xff);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_0, 0x0f);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_1, 0x0f);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_2, 0x0f);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_3, 0x0f);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_4, 0x0f);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_5, 0x0f);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_6, 0x0f);
	max72xx_transmit(max72xx, MAX72XX_DIGIT_7, 0x0f);

	return result;

}

MAX72XX_result_t max72xx_set_digit(MAX72XX_HandleTypeDef *max72xx, uint8_t digit, uint8_t value) {
	MAX72XX_result_t result = MAX72XX_Ok;

	max72xx_transmit(max72xx, digit, value);

	return result;

}

MAX72XX_result_t max72xx_display_number(MAX72XX_HandleTypeDef *max72xx, uint32_t number) {
	MAX72XX_result_t result = MAX72XX_Ok;

	uint8_t digit_count = log10(number);

	for (int i = max72xx->digits; i > 0; --i) {
		if (i <= digit_count) {
			uint32_t digit = ((uint32_t)(number / pow(10, i))) % 10;
			max72xx_set_digit(max72xx, i, (uint8_t) digit);
		} else {
			max72xx_set_digit(max72xx, i, 0x0f);
		}
	}

	return result;
}
