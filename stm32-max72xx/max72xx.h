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

#define MAX72XX_NO_OP 0x00
#define MAX72XX_DIGIT_0 0x01
#define MAX72XX_DIGIT_1 0x02
#define MAX72XX_DIGIT_2 0x03
#define MAX72XX_DIGIT_3 0x04
#define MAX72XX_DIGIT_4 0x05
#define MAX72XX_DIGIT_5 0x06
#define MAX72XX_DIGIT_6 0x07
#define MAX72XX_DIGIT_7 0x08
#define MAX72XX_DECODE_MODE 0x09
#define MAX72XX_INTENSITY 0x0a
#define MAX72XX_SCAN_LIMIT 0x0b
#define MAX72XX_SHUTDOWN 0x0c
#define MAX72XX_DISPLAY_TEST 0x0f

typedef struct {
	SPI_HandleTypeDef *spiHandle;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	uint8_t digits;
} MAX72XX_HandleTypeDef;

typedef enum {
	MAX72XX_Ok,   // 0
	MAX72_Err,    // 1
	MAX72_Timeout // 2
} MAX72XX_result_t;

MAX72XX_result_t max72xx_init(MAX72XX_HandleTypeDef *max72xx, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t digits);

#endif /* MAX72XX_H_ */
