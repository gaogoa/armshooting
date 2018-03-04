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

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "Common.h"
#include "LCD.h"

const int GRAPHIC_WIDTH       = LCD_WIDTH;
const int GRAPHIC_HEIGHT      = 256;
const int GRAPHIC_BUFF_HEIGHT = 16;
const int GRAPHIC_BG_SIZE     = 16;
const int BG_SIZE             = 16;
const int BG_COLS             = GRAPHIC_WIDTH / BG_SIZE;
const int BG_ROWS             = GRAPHIC_HEIGHT / BG_SIZE;

extern uint16_t bitmapBuff[GRAPHIC_WIDTH * GRAPHIC_BUFF_HEIGHT];

#endif

