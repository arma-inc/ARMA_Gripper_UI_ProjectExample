// ARMA Gripper UI
#pragma once
/*
  Software License Agreement (BSD-1-Clause)

  Copyright (c) 2024 ARMA Inc. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef ARDUINO_M5STACK_Core2
#elif ARDUINO_M5STACK_CORES3
#else
#error "Device is not supported!"
#endif

#include <M5Unified.h>

#define LCD M5.Display

void ui_set_text_params(uint16_t fgcolor = LIGHTGREY,
                        uint16_t bgcolor = BLACK,
                        uint8_t font = 1,
                        uint8_t size = 2,
                        uint8_t datum = TL_DATUM);

void ui_clear_rest_of_line();

#define BUTTON_GREY (0x4A69)

typedef struct ButtonStateFormat_s {
    uint8_t textSize;
    uint8_t textFont;
    uint8_t frameWidth;
    uint16_t colorShape;
    uint16_t colorText;
    uint16_t colorFrame;
} ButtonStateFormat_t;

class UIButton {
  public:
    UIButton(int32_t x, int32_t y, int32_t width, int32_t height, const char* label);
    virtual ~UIButton();

    virtual void update(bool force_draw = false);
    bool isPressed() const { return is_pressed; }
    bool isReleased() const { return !is_pressed; }
    bool isChangeToPressed() const { return is_pressed && is_change; }
    bool isChangeToReleased() const { return !is_pressed && is_change; }

    const char* label;

    const ButtonStateFormat_t* formatPressed = nullptr;   // set nullptr for default format
    const ButtonStateFormat_t* formatReleased = nullptr;  // set nullptr for default format

  protected:
    virtual void updateTouch();
    virtual void draw();

    int32_t x, y, width, height;
    bool is_pressed = false;
    bool is_change = false;
};
