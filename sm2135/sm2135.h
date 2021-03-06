// Copyright 2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __SM2135_H__
#define __SM2135_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mdf_common.h"
#include "driver/ledc.h"


typedef enum{
    RGB_CURR_9_5MA,
    RGB_CURR_14_0MA,
    RGB_CURR_18_5MA,
    RGB_CURR_23_0MA,
    RGB_CURR_27_5MA,
    RGB_CURR_32_0MA,
    RGB_CURR_36_5MA,
    RGB_CURR_41_0MA,
    RGB_CURR_MAX,
}rgb_curr_t;

typedef enum{
    WY_CURR_0MA,
    WY_CURR_4_8MA,
    WY_CURR_9_6MA,
    WY_CURR_14_4MA,
    WY_CURR_20_2MA,
    WY_CURR_24_0MA,
    WY_CURR_28_8MA,
    WY_CURR_33_6MA,
    WY_CURR_38_4MA,
    WY_CURR_43_2MA,
    WY_CURR_48_0MA,
    WY_CURR_52_8MA,
    WY_CURR_57_6MA,
    WY_CURR_62_4MA,
    WY_CURR_67_2MA,
    WY_CURR_72_0MA,
    WY_CURR_MAX,
}wy_curr_t;


#define GAMMA_CORRECTION 0.8                               /**< Gamma curve parameter */
#define GAMMA_TABLE_SIZE 256                               /**< Gamma table size, used for led fade*/
#define DUTY_SET_CYCLE (20)                                /**< Set duty cycle */

/**
  * @brief Initialize and set the ledc timer for the iot led
  *
  * @param timer_num The timer index of ledc timer group used for iot led
  *     This parameter can be one of LEDC_TIMER_x where x can be (0 .. 3)
  *
  * @param speed_mode speed mode of ledc timer
  *     This parameter can be one of LEDC_x_SPEED_MODE where x can be (LOW, HIGH)
  *
  * @param freq_hz frequency of ledc timer
  *     This parameter must be less than 5000
  *
  * @return
  *	    - MDF_OK if sucess
  *     - MDF_ERR_INVALID_ARG Parameter error
  *     - MDF_FAIL Can not find a proper pre-divider number base on the given frequency
  *         and the current duty_resolution.
*/
mdf_err_t iot_led_init(ledc_timer_t timer_num, ledc_mode_t speed_mode, uint32_t freq_hz);

/**
  * @brief DeInitializes the iot led and free resource
  *
  * @return
  *	    - MDF_OK if sucess
*/
mdf_err_t iot_led_deinit();

/**
  * @brief Set the ledc channel used by iot led and associate the gpio port used
  *     for output
  *
  * @param channel The ledc channel
  *     This parameter can be LEDC_CHANNEL_x where x can be (0 .. 15)
  * @param gpio_num the ledc output gpio_num
  *     This parameter can be GPIO_NUM_x where x can be (0, 33)
  *
  * @note If the operation of esp32 depends on SPI FLASH or PSRAM, then these related
  *     pins should not be set to output.
  *
  * @return
  *	    - MDF_OK if sucess
  *     - MDF_ERR_INVALID_ARG Parameter error
  *	    - MDF_ERR_NOT_INIT if lot_led_init() is not called yet
*/
mdf_err_t iot_led_regist_channel(ledc_channel_t channel, gpio_num_t gpio_num);

/**
  * @brief Returns the channel value
  * @note before calling this function, you need to call iot_led_regist_channel() to
  *     set the channel
  *
  * @param channel The ledc channel
  *     This parameter can be LEDC_CHANNEL_x where x can be (0 .. 15)
  * @param dst The address where the channel value is stored
  * @return
  *     - MDF_OK if sucess
  *	    - MDF_ERR_INVALID_ARG if dst is NULL
  *	    - MDF_ERR_NOT_INIT if lot_led_init() is not called yet
*/
mdf_err_t iot_led_get_channel(ledc_channel_t channel, uint8_t *dst);

/**
  * @brief Set the fade state for the specified channel
  * @note before calling this function, you need to call iot_led_regist_channel() to
  *     set the channel
  *
  * @param channel The ledc channel
  *     This parameter can be LEDC_CHANNEL_x where x can be (0 .. 15)
  * @param value The target output brightness of iot led
  *     This parameter can be (0 .. 255)
  * @param fade_ms The time from the current value to the target value
  * @return
  *	    - MDF_OK if sucess
  *	    - MDF_ERR_NOT_INIT if lot_led_init() is not called yet
*/
esp_err_t iot_led_set_channel(ledc_channel_t channel, uint8_t value, uint32_t fade_ms);

/**
  * @brief Set the blink state or loop fade for the specified channel
  * @note before calling this function, you need to call iot_led_regist_channel() to
  *     set the channel
  *
  * @param channel The ledc channel
  *     This parameter can be LEDC_CHANNEL_x where x can be (0 .. 15)
  * @param value The output brightness of iot led
  *     This parameter can be (0 .. 255)
  * @param period_ms Blink cycle
  * @param fade_flag select loop fade or blink
  *     1 for loop fade
  *     0 for blink
  * @return
  *	    - MDF_OK if sucess
  *	    - MDF_ERR_NOT_INIT if lot_led_init() is not called yet
*/
esp_err_t iot_led_start_blink(ledc_channel_t channel, uint8_t value, uint32_t period_ms, bool fade_flag);

/**
  * @brief Stop the blink state or loop fade for the specified channel
  *
  * @param channel The ledc channel
  *     This parameter can be LEDC_CHANNEL_x where x can be (0 .. 15)
  * @return
  *	    - MDF_OK if sucess
  *	    - MDF_ERR_NOT_INIT if lot_led_init() is not called yet
*/
esp_err_t iot_led_stop_blink(ledc_channel_t channel);

/**
  * @brief Set the specified gamma_table to control the fade effect, usually
  *     no need to set
  *
  * @param gamma_table[GAMMA_TABLE_SIZE] Expected gamma table value
  *
  * @note  Gamma_table is the dimming curve used by the iot_led driver.
  *     The element type is uint16_t. Each element is treated as a binary
  *     fixed-point number. The decimal point is before the eighth bit
  *     and after the ninth bit, so the range of expressions can be
  *     0x00.00 ~ 0xff.ff.
  * @note default gamma_table is created in iot_led_init()
  *
  * @return
  *	    - MDF_OK if sucess
  *	    - MDF_ERR_NOT_INIT if lot_led_init() is not called yet
*/
mdf_err_t iot_led_set_gamma_table(const uint16_t gamma_table[GAMMA_TABLE_SIZE]);


#ifdef __cplusplus
}
#endif

#endif /**< __SM2135_H__ */
