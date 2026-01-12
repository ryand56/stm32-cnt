#include "main.h"
#include <Project/projectMain.h>
#include <array>
#include <cstdio>
#include <span>

struct Led
{
    explicit Led(GPIO_TypeDef* t_gpio, uint16_t t_pin)
        : gpio(t_gpio)
        , pin(t_pin)
    {
    }
    void on()
    {
        HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
    }
    void off()
    {
        HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
    }
    void toggle()
    {
        HAL_GPIO_TogglePin(gpio, pin);
    }
    auto state() -> bool
    {
        return HAL_GPIO_ReadPin(gpio, pin);
    }

private:
    GPIO_TypeDef* gpio;
    uint16_t pin;
};

void projectMain()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitTypeDef GPIO_InitStructLED = {0};
    GPIO_InitStructLED.Pin = GPIO_PIN_0;
    GPIO_InitStructLED.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructLED.Pull = GPIO_NOPULL;
    GPIO_InitStructLED.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructLED);

    Led led(GPIOC, GPIO_PIN_6);
    Led circuitLed(GPIOA, GPIO_PIN_0);

    while (true)
    {
        led.toggle();
        HAL_Delay(2000);

	circuitLed.toggle();
	HAL_Delay(2000);
    }
}
