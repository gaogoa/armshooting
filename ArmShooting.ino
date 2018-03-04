/*
 * The MIT License
 * 
 * Copyright (c) 2018 gaogoa.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * Main routine.
 */

#include "LCD.h"
#include "Graphics.h"
#include "Sprite.h"
#include "TextArea.h"
#include "Object.h"
#include "Common.h"

void setup() {
  pinMode(PORT_SW_LEFT, INPUT_PULLUP);
  pinMode(PORT_SW_RIGHT, INPUT_PULLUP);
  pinMode(PORT_SW_FIRE, INPUT_PULLUP);

  LCD_Init();
  Sprites_Init();
  s_game->init();
}

static unsigned long last = 0;
static unsigned long sync_t = 0;

void loop() {
  unsigned long t = millis();
  if ((t - sync_t) > (1000 / 60)) {
    s_game->move();
    Sprites_Update();
    unsigned long t2 = millis();
    s_text->clear();
    s_game->drawTextPart(last > 0 ? (1000 / last) : 0);
    s_text->update();
    last = t2 - t;
    sync_t = t;
  }
}

