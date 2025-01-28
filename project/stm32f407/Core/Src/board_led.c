#include "board_led.h"
#include "main.h"

#define LED_NUM 3
static uint16_t ledPin[LED_NUM] = 
{
    GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, 
};

void BoardLed_LightOn(unsigned index)
{
    if (index < LED_NUM)
    {
        HAL_GPIO_WritePin(GPIOE, ledPin[index], GPIO_PIN_RESET);
    }
}

void BoardLed_LightOff(unsigned index)
{
    if (index < LED_NUM)
    {
        HAL_GPIO_WritePin(GPIOE, ledPin[index], GPIO_PIN_SET);
    }
}

void BoardLed_Toggle(unsigned index)
{
    if (index < LED_NUM)
    {
        HAL_GPIO_TogglePin(GPIOE, ledPin[index]);
    }
}
