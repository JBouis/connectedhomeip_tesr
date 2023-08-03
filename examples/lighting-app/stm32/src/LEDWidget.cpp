/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "main.h"
#include "LEDWidget.h"
#include "AppConfig.h"
// #include "ColorFormat.h"
// #include "led_strip.h"

static const char * TAG = "LEDWidget";

void LEDWidget::Init(GPIO_TypeDef *BUTTON_GPIO_PORT, uint16_t BUTTON_GPIO_PIN)
{
    m_GPIO_PORT = BUTTON_GPIO_PORT;
    m_GPIO_PIN = BUTTON_GPIO_PIN;

    STM32_LOG("m_GPIO_PORT (s) = %s", m_GPIO_PORT);
    STM32_LOG("m_GPIO_PIN (s) = %s", m_GPIO_PIN);

    STM32_LOG("m_GPIO_PORT (d) = %d", m_GPIO_PORT);
    STM32_LOG("m_GPIO_PIN (d) = %d", m_GPIO_PIN);

    mState      = false;
    mBrightness = UINT8_MAX;
}

void LEDWidget::GetNameOfGPIO(void)
{
    // #ifdef GPIOA
    // if(m_GPIO_PORT == GPIOA_BASE) m_GPIO_PORT_NAME = "A";
    // #endif
    // if(m_GPIO_PORT == GPIOB_BASE) m_GPIO_PORT_NAME = "B";
    // if(m_GPIO_PORT == GPIOC_BASE) m_GPIO_PORT_NAME = "C";
    // if(m_GPIO_PORT == GPIOD_BASE) m_GPIO_PORT_NAME = "D";
    // if(m_GPIO_PORT == GPIOE_BASE) m_GPIO_PORT_NAME = "E";
    // if(m_GPIO_PORT == GPIOF_BASE) m_GPIO_PORT_NAME = "F";
    // if(m_GPIO_PORT == GPIOG_BASE) m_GPIO_PORT_NAME = "G";
    // if(m_GPIO_PORT == GPIOH_BASE) m_GPIO_PORT_NAME = "H";
    // if(m_GPIO_PORT == GPIOI_BASE) m_GPIO_PORT_NAME = "I";
    // if(m_GPIO_PORT == GPIOJ_BASE) m_GPIO_PORT_NAME = "J";
    // if(m_GPIO_PORT == GPIOK_BASE) m_GPIO_PORT_NAME = "K";
}

void LEDWidget::Set(bool state)
{
    STM32_LOG("Setting state to %d", state ? 1 : 0);
    if (state == mState)
        return;

    mState = state;

    DoSet();
}

void LEDWidget::Toggle()
{
    STM32_LOG("Toggling state to %d", !mState);
    mState = !mState;

    DoSet();
}

void LEDWidget::SetBrightness(uint16_t brightness)
{
    STM32_LOG("Setting brightness to %d", brightness);
    if (brightness == mBrightness)
        return;

    mBrightness = brightness;

    DoSet();
}

uint8_t LEDWidget::GetLevel()
{
    return this->mBrightness;
}

bool LEDWidget::IsTurnedOn()
{
    return this->mState;
}

// #if CONFIG_LED_TYPE_RMT
// void LEDWidget::SetColor(uint8_t Hue, uint8_t Saturation)
// {
//     if (Hue == mHue && Saturation == mSaturation)
//         return;

//     mHue        = Hue;
//     mSaturation = Saturation;

//     DoSet();
// }
// #endif // CONFIG_LED_TYPE_RMT

void LEDWidget::DoSet(void)
{
    uint8_t brightness = mState ? mBrightness : 0;

    HAL_GPIO_WritePin(m_GPIO_PORT, m_GPIO_PIN, (mState == true) ? GPIO_PIN_SET : GPIO_PIN_RESET);

// #if CONFIG_LED_TYPE_RGB

// #else

    //STM32_LOG("DoSet to GPIO number %d", mGPIONum);

// #endif // CONFIG_LED_TYPE_RMT
STM32_LOG("DoSet to GPIO number");
}
