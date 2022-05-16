/*
 * max72xx.h
 *
 * Minimalistic library for the MAX7219 and MAX7221 7 segment LED drivers
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 *
 */

#ifndef MAX72XX_H_
#define MAX72XX_H_

typedef struct {
	SPI_HandleTypeDef *spiHandle;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
} MAX72XX_HandleTypeDef;

typedef enum {
	MAX72XX_Ok,   // 0
    MAX72_Err,    // 1
    MAX72_Timeout // 2
} MAX72XX_result_t;

MAX72XX_result_t max72xx_init(MAX72XX_HandleTypeDef *max72xx, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);

#endif /* MAX72XX_H_ */
