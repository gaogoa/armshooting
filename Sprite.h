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

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Common.h"

class Sprite {
  public:
    bool use = false;
    bool show = false;
    int x = 0;
    int y = 0;
    int w = 16;
    int h = 16;
    const uint16_t *image = NULL;
  public:
    bool hit(Sprite *other);
};

void Sprites_Init();
void Sprites_Update();
Sprite *Sprites_Allocate();

void Bg_SetEnable(bool on);
void Bg_Fill(uint8_t ch);
void Bg_Set(int x, int y, uint8_t ch);
void Bg_SetCharacter(uint8_t ch, const uint16_t *image);

#endif

