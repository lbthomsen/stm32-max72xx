/*
 * max72xx.h
 *
 * Minimalistic library for the MAX7219 and MAX7221 7 segment LED drivers
 *
 * At the moment this has only been tested with MAX7219 but the only difference should be the
 * way the CS pin is interpreted and the library should use it in a way that work with both.
 *
 * Daisy chaining multiple devices is also not currently supported!  I only got one of these
 * available.
 *
 * See: https://stm32world.com/wiki/STM32_MAX7219/MAX7221
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 *
 */

#ifndef MAX72XX_H_
#define MAX72XX_H_

#define MAX72XX_NO_OP 			0x00
#define MAX72XX_DIGIT_0 		0x01
#define MAX72XX_DIGIT_1 		0x02
#define MAX72XX_DIGIT_2 		0x03
#define MAX72XX_DIGIT_3 		0x04
#define MAX72XX_DIGIT_4 		0x05
#define MAX72XX_DIGIT_5 		0x06
#define MAX72XX_DIGIT_6 		0x07
#define MAX72XX_DIGIT_7 		0x08
#define MAX72XX_DECODE_MODE 	0x09
#define MAX72XX_INTENSITY 		0x0a
#define MAX72XX_SCAN_LIMIT 		0x0b
#define MAX72XX_SHUTDOWN 		0x0c
#define MAX72XX_DISPLAY_TEST 	0x0f

typedef struct {
    SPI_HandleTypeDef *spiHandle;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    uint8_t awake;
    uint8_t digits;
    uint8_t intensity;
    uint8_t decode;
} MAX72XX_HandleTypeDef;

typedef enum {
    MAX72XX_Ok,   // 0
    MAX72XX_Err,  // 1
} MAX72XX_result_t;

extern const uint8_t max72xx_font[];

MAX72XX_result_t max72xx_init(MAX72XX_HandleTypeDef *max72xx, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);

MAX72XX_result_t max72xx_shutdown(MAX72XX_HandleTypeDef *max72xx);

MAX72XX_result_t max72xx_wakeup(MAX72XX_HandleTypeDef *max72xx);

MAX72XX_result_t max72xx_digits(MAX72XX_HandleTypeDef *max72xx, uint8_t digits);

MAX72XX_result_t max72xx_intensity(MAX72XX_HandleTypeDef *max72xx, uint8_t intensity);

MAX72XX_result_t max72xx_decode(MAX72XX_HandleTypeDef *max72xx, uint8_t decode);

MAX72XX_result_t max72xx_set_digit(MAX72XX_HandleTypeDef *max72xx, uint8_t digit, uint8_t value);

MAX72XX_result_t max72xx_display_number(MAX72XX_HandleTypeDef *max72xx, uint32_t number);

#endif /* MAX72XX_H_ */
/*
 * vim: ts=4 et nowrap autoindent
 */

