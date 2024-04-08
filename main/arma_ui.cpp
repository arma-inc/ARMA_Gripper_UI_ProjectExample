// ARMA Gripper UI

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

#include "arma_ui.h"

void ui_set_text_params(
    uint16_t fgcolor, uint16_t bgcolor, uint8_t font, uint8_t size, uint8_t datum) {
    LCD.setTextColor(fgcolor, bgcolor);
    LCD.setTextFont(font);
    LCD.setTextSize(size);
    LCD.setTextDatum(datum);
}

void ui_clear_rest_of_line() {
    LCD.setColor(BLACK);
    LCD.fillRect(
        LCD.getCursorX(), LCD.getCursorY(), LCD.width() - LCD.getCursorX(), LCD.fontHeight());
}

static const ButtonStateFormat_t defaultButtonFormatPressed = { .textSize = 2,
                                                                .textFont = 1,
                                                                .frameWidth = 1,
                                                                .colorShape = RED,
                                                                .colorText = WHITE,
                                                                .colorFrame = BUTTON_GREY };

static const ButtonStateFormat_t defaultButtonFormatReleased = { .textSize = 2,
                                                                 .textFont = 1,
                                                                 .frameWidth = 1,
                                                                 .colorShape = BUTTON_GREY,
                                                                 .colorText = WHITE,
                                                                 .colorFrame = WHITE };

UIButton::UIButton(int32_t x, int32_t y, int32_t width, int32_t height, const char* label)
    : label(label), x(x), y(y), width(width), height(height) {}

UIButton::~UIButton() {}

void UIButton::update(bool force_draw) {
    updateTouch();
    // static int ls = -1;
    // int s = is_change << 1 + is_pressed;
    // if (ls != s) {
    //     Serial.printf("BTN(%s): %c%c\n", label, is_change ? 'C' : 'c', is_pressed ? 'P' : 'p');
    //     ls = s;
    // }
    if (is_change || force_draw) draw();
}

void UIButton::updateTouch() {
    auto count = M5.Touch.getCount();
    if (!count) {
        is_change = is_pressed;
        is_pressed = false;
        return;
    }
    if (count > 1) return;
    auto tp = M5.Touch.getTouchPointRaw();
    bool press = (tp.x >= x) && (tp.x < (x + width)) && (tp.y >= y) && (tp.y < (y + height));
    is_change = (press ^ is_pressed);
    is_pressed = press;
}

void UIButton::draw() {
    const ButtonStateFormat_t* bfp = (formatPressed) ? formatPressed : &defaultButtonFormatPressed;
    const ButtonStateFormat_t* bfr =
        (formatReleased) ? formatReleased : &defaultButtonFormatReleased;
    const ButtonStateFormat_t* bf = is_pressed ? bfp : bfr;
    // Serial.printf("BTN(%s): draw(%d, %d, %d, %d)\n", label, x, y, width, height);

    // draw Rect Rounded Shape
    int r = std::min(width, height) / 10 + 1;
    LCD.fillRoundRect(x, y, width, height, r, bf->colorFrame);
    if ((bf->frameWidth > 0) && (bf->colorShape != bf->colorFrame)) {
        uint16_t frame2w = ((uint16_t)bf->frameWidth) * 2;
        LCD.fillRoundRect(x + bf->frameWidth,
                          y + bf->frameWidth,
                          width - frame2w,
                          height - frame2w,
                          r,
                          bf->colorShape);
    }

    // draw label
    if (label) {
        ui_set_text_params(bf->colorText, bf->colorText, bf->textFont, bf->textSize, MC_DATUM);
        LCD.drawString(label, x + width / 2 + 1, y + height / 2 + 1);
    }
}
