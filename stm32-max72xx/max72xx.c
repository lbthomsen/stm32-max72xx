/*
 * max72xx.c
 *
 * Minimalistic library for the MAX7219 and MAX7221 7 segment LED drivers
 *
 * See: https://stm32world.com/wiki/STM32_MAX7219/MAX7221
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 *
 */

#include <math.h>
#include "main.h"
#include "max72xx.h"

/*
 *
 * Private Variables
 *
 */

const uint8_t max72xx_font[] = {
        0b01111110, // 0
        0b00110000, // 1
        0b01101101, // 2
        0b01111001, // 3
        0b00110011, // 4
        0b01011011, // 5
        0b01011111, // 6
        0b01110000, // 7
        0b01111111, // 8
        0b01111011, // 9
        0b00000001, // -
        0b01001111, // E
        0b00110111, // H
        0b00001110, // L
        0b01100111, // P
        0x00000000, // blank
};

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

    uint8_t data[2] = { address, value };

    cs_enable(max72xx);
    if (HAL_SPI_Transmit(max72xx->spiHandle, (uint8_t*) &data, 2, HAL_MAX_DELAY) != HAL_OK)
        result = MAX72XX_Err;
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
MAX72XX_result_t max72xx_init(MAX72XX_HandleTypeDef *max72xx, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin) {

    MAX72XX_result_t result = MAX72XX_Ok;

    max72xx->spiHandle = hspi;
    max72xx->cs_port = cs_port;
    max72xx->cs_pin = cs_pin;

//    max72xx_transmit(max72xx, MAX72XX_DIGIT_0, 0x0f);
//    max72xx_transmit(max72xx, MAX72XX_DIGIT_1, 0x0f);
//    max72xx_transmit(max72xx, MAX72XX_DIGIT_2, 0x0f);
//    max72xx_transmit(max72xx, MAX72XX_DIGIT_3, 0x0f);
//    max72xx_transmit(max72xx, MAX72XX_DIGIT_4, 0x0f);
//    max72xx_transmit(max72xx, MAX72XX_DIGIT_5, 0x0f);
//    max72xx_transmit(max72xx, MAX72XX_DIGIT_6, 0x0f);
//    max72xx_transmit(max72xx, MAX72XX_DIGIT_7, 0x0f);

    return result;

}

MAX72XX_result_t max72xx_shutdown(MAX72XX_HandleTypeDef *max72xx) {
    MAX72XX_result_t result = MAX72XX_Ok;
    if (max72xx_transmit(max72xx, MAX72XX_SHUTDOWN, 0) != MAX72XX_Ok) {
        return MAX72XX_Err;
    }
    max72xx->awake = 0;
    return result;
}

MAX72XX_result_t max72xx_wakeup(MAX72XX_HandleTypeDef *max72xx) {
    MAX72XX_result_t result = MAX72XX_Ok;
    if (max72xx_transmit(max72xx, MAX72XX_SHUTDOWN, 1) != MAX72XX_Ok) {
        return MAX72XX_Err;
    }
    max72xx->awake = 1;
    return result;
}

MAX72XX_result_t max72xx_digits(MAX72XX_HandleTypeDef *max72xx, uint8_t digits) {
    MAX72XX_result_t result = MAX72XX_Ok;
    if (max72xx_transmit(max72xx, MAX72XX_SCAN_LIMIT, digits - 1) != MAX72XX_Ok) {
        return MAX72XX_Err;
    }
    max72xx->digits = digits;
    return result;
}

MAX72XX_result_t max72xx_intensity(MAX72XX_HandleTypeDef *max72xx, uint8_t intensity) {
    MAX72XX_result_t result = MAX72XX_Ok;
    if (max72xx_transmit(max72xx, MAX72XX_INTENSITY, intensity) != MAX72XX_Ok) {
        return MAX72XX_Err;
    }
    max72xx->intensity = intensity;
    return result;
}

MAX72XX_result_t max72xx_decode(MAX72XX_HandleTypeDef *max72xx, uint8_t decode) {
    MAX72XX_result_t result = MAX72XX_Ok;
    if (max72xx_transmit(max72xx, MAX72XX_DECODE_MODE, decode) != MAX72XX_Ok) {
        return MAX72XX_Err;
    }
    max72xx->decode = decode;
    return result;
}

MAX72XX_result_t max72xx_set_digit(MAX72XX_HandleTypeDef *max72xx, uint8_t digit, uint8_t value) {
    MAX72XX_result_t result = MAX72XX_Ok;

    uint8_t write_value = value;
    uint8_t decode_digit = 0x01 & (max72xx->decode >> (digit - 1));

    // Check if decode for the digit is enabled
    if (decode_digit == 0) {
        write_value = max72xx_font[value];
    }

    if (max72xx_transmit(max72xx, digit, write_value) != MAX72XX_Ok) {
        result = MAX72XX_Err;
    }

    return result;

}

MAX72XX_result_t max72xx_display_number(MAX72XX_HandleTypeDef *max72xx, uint32_t number) {
    MAX72XX_result_t result = MAX72XX_Ok;

    uint8_t digit_count = log10(number) + 1;

    for (int i = max72xx->digits; i > 0; --i) {
        if (i <= digit_count) {
            uint32_t digit = ((uint32_t) (number / pow(10, i - 1))) % 10;
            max72xx_set_digit(max72xx, i, (uint8_t) digit);
        } else {
            max72xx_set_digit(max72xx, i, 0x0f);
        }
    }

    return result;
}

/*
 * vim: ts=4 et nowrap autoindent
 */
