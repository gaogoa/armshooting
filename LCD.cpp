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

/*
 * Control the LCD. 
 * The contents are simple, it only has initialization and frame buffer transfer.
 */

#include "LCD.h"

const int PORT_LCD_DB15   = PB15; // R4
const int PORT_LCD_DB14   = PB14; // R3
const int PORT_LCD_DB13   = PB13; // R2
const int PORT_LCD_DB12   = PB12; // R1
const int PORT_LCD_DB11   = PB11; // R0
const int PORT_LCD_DB10   = PB10; // G5
const int PORT_LCD_DB9    = PB9;  // G4
const int PORT_LCD_DB8    = PB8;  // G3
const int PORT_LCD_DB7    = PA7;  // G2
const int PORT_LCD_DB6    = PA6;  // G1
const int PORT_LCD_DB5    = PA5;  // G0
const int PORT_LCD_DB4    = PA4;  // B4
const int PORT_LCD_DB3    = PA3;  // B3
const int PORT_LCD_DB2    = PA2;  // B2
const int PORT_LCD_DB1    = PA1;  // B1
const int PORT_LCD_DB0    = PA0;  // B0

const int PORT_LCD_RS   = PB5;
const int PORT_LCD_WR   = PB6;
const int PORT_LCD_RD   = PB7;

const int PORT_LCD_REST = PC13;
const int PORT_LCD_CS   = PC14;

// * Notes on changing GPIO assignment *
// The bit pattern of the part written to the BSRR register depends on the GPIO port number

#define LCD_RS_0 (GPIOB->regs->BSRR = 0b0000000000100000 << 16)
#define LCD_RS_1 (GPIOB->regs->BSRR = 0b0000000000100000)

#define LCD_WR_0 (GPIOB->regs->BSRR = 0b0000000001000000 << 16)
#define LCD_WR_1 (GPIOB->regs->BSRR = 0b0000000001000000)

#define LCD_RD_0 (GPIOB->regs->BSRR = 0b0000000010000000 << 16)
#define LCD_RD_1 (GPIOB->regs->BSRR = 0b0000000010000000)

#define LCD_CS_0 (GPIOC->regs->BSRR = 0b0100000000000000 << 16)
#define LCD_CS_1 (GPIOC->regs->BSRR = 0b0100000000000000)

#define LCD_REST_0 (GPIOC->regs->BSRR = 0b0010000000000000 << 16)
#define LCD_REST_1 (GPIOC->regs->BSRR = 0b0010000000000000)

static void AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void LCD_Init_Sub(void);

// Execute LCD_RS_0/1 beforehand
INLINE void LCD_WR_BUS(uint16_t d)
{
  GPIOA->regs->ODR = d & 0xff;
  GPIOB->regs->BSRR = (0b1111111100000000 << 16);
  GPIOB->regs->BSRR = d & 0xff00;
  LCD_WR_0;
  // asm volatile ("nop\n");
  LCD_WR_1;
}

INLINE void LCD_WR_DATA(uint16_t da)
{
  LCD_RS_1;
  LCD_WR_BUS(da);
}

INLINE void LCD_WR_REG(uint16_t da)
{
  LCD_RS_0;
  LCD_WR_BUS(da);
}

void AddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  // Column Address Set (2Ah)
  LCD_WR_REG(0x2A);
  LCD_RS_1;
  LCD_WR_BUS(x1 >> 8);
  LCD_WR_BUS(x1);
  LCD_WR_BUS(x2 >> 8);
  LCD_WR_BUS(x2);

  // Page Address Set (2Bh)
  LCD_WR_REG(0x2B);
  LCD_RS_1;
  LCD_WR_BUS(y1 >> 8);
  LCD_WR_BUS(y1);
  LCD_WR_BUS(y2 >> 8);
  LCD_WR_BUS(y2);

  // Memory Write (2Ch)
  LCD_WR_REG(0x2c);
}

void LCD_Init(void)
{
  pinMode(PORT_LCD_DB15, OUTPUT);
  pinMode(PORT_LCD_DB14, OUTPUT);
  pinMode(PORT_LCD_DB13, OUTPUT);
  pinMode(PORT_LCD_DB12, OUTPUT);
  pinMode(PORT_LCD_DB11, OUTPUT);
  pinMode(PORT_LCD_DB10, OUTPUT);
  pinMode(PORT_LCD_DB9, OUTPUT);
  pinMode(PORT_LCD_DB8, OUTPUT);
  pinMode(PORT_LCD_DB7, OUTPUT);
  pinMode(PORT_LCD_DB6, OUTPUT);
  pinMode(PORT_LCD_DB5, OUTPUT);
  pinMode(PORT_LCD_DB4, OUTPUT);
  pinMode(PORT_LCD_DB3, OUTPUT);
  pinMode(PORT_LCD_DB2, OUTPUT);
  pinMode(PORT_LCD_DB1, OUTPUT);
  pinMode(PORT_LCD_DB0, OUTPUT);

  pinMode(PORT_LCD_RS, OUTPUT);
  pinMode(PORT_LCD_WR, OUTPUT);
  pinMode(PORT_LCD_RD, OUTPUT);

  pinMode(PORT_LCD_REST, OUTPUT);
  pinMode(PORT_LCD_CS, OUTPUT);

  LCD_CS_1;
  LCD_RD_1;
  LCD_WR_1;
  LCD_REST_0;
  delay(20);
  LCD_REST_1;
  delay(20);
  LCD_CS_0;

  delay(100);

  LCD_Init_Sub();

  AddressSet(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
  LCD_RS_1;
  for (int b = 0; b < LCD_HEIGHT * LCD_WIDTH; b++) {
    LCD_WR_BUS(0);
  }
}

void LCD_Init_Sub(void)
{
  // Power control B (CFh)
  LCD_WR_REG(0xcf);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0xc1);
  LCD_WR_DATA(0x30);

  // Power on sequence control (EDh)
  LCD_WR_REG(0xed);
  LCD_WR_DATA(0x64);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x12);
  LCD_WR_DATA(0x81);

  // Power control A (CBh)
  LCD_WR_REG(0xcb);
  LCD_WR_DATA(0x39);
  LCD_WR_DATA(0x2c);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x34);
  LCD_WR_DATA(0x02);

  // Driver timing control B (EAh)
  LCD_WR_REG(0xea);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);

  // Driver timing control A (E8h)
  LCD_WR_REG(0xe8);
  LCD_WR_DATA(0x85);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x79);

  // Power Control 1 (C0h)
  LCD_WR_REG(0xC0); //Power control
  LCD_WR_DATA(0x23); //VRH[5:0]

  // Power Control 2 (C1h)
  LCD_WR_REG(0xC1); //Power control
  LCD_WR_DATA(0x11); //SAP[2:0];BT[3:0]

  // ?? = POWER CONTROL 3 (in Normal mode/ Full colors)
  LCD_WR_REG(0xC2);
  LCD_WR_DATA(0x11);

  // VCOM Control 1(C5h)
  LCD_WR_REG(0xC5); //VCM control
  LCD_WR_DATA(0x3d);
  LCD_WR_DATA(0x30);

  // VCOM Control 2(C7h)
  LCD_WR_REG(0xc7);
  LCD_WR_DATA(0xaa);

  // COLMOD: Pixel Format Set (3Ah)
  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x55);  // DPI=101, DBI=101
  // 65K color: 16-bit/pixel (RGB 5-6-5 bits input)

  // Memory Access Control (36h)
  LCD_WR_REG(0x36); // Memory Access Control
  LCD_WR_DATA(0x08);

  // Frame Rate Control (In Normal Mode/Full Colors) (B1h)
  LCD_WR_REG(0xB1); // Frame Rate Control
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x11);

  LCD_WR_REG(0xB6); // Display Function Control
  LCD_WR_DATA(0x0a);
  LCD_WR_DATA(0xa2);

  // Enable 3G (F2h)
  LCD_WR_REG(0xF2); // 3Gamma Function Disable
  LCD_WR_DATA(0x00);

  // Pump ratio control (F7h)
  LCD_WR_REG(0xF7);
  LCD_WR_DATA(0x20);

  // ??
  LCD_WR_REG(0xF1);
  LCD_WR_DATA(0x01);
  LCD_WR_DATA(0x30);

  // Gamma Set (26h)
  LCD_WR_REG(0x26); //Gamma curve selected
  LCD_WR_DATA(0x01);

  // Positive Gamma Correction (E0h)
  LCD_WR_REG(0xE0); //Set Gamma
  LCD_WR_DATA(0x0f);
  LCD_WR_DATA(0x3f);
  LCD_WR_DATA(0x2f);
  LCD_WR_DATA(0x0c);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x0a);
  LCD_WR_DATA(0x53);
  LCD_WR_DATA(0xd5);
  LCD_WR_DATA(0x40);
  LCD_WR_DATA(0x0a);
  LCD_WR_DATA(0x13);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x08);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x00);

  // Negative Gamma Correction (E1h)
  LCD_WR_REG(0xE1); //Set Gamma
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x0f);
  LCD_WR_DATA(0x05);
  LCD_WR_DATA(0x2c);
  LCD_WR_DATA(0xa2);
  LCD_WR_DATA(0x3f);
  LCD_WR_DATA(0x05);
  LCD_WR_DATA(0x0e);
  LCD_WR_DATA(0x0c);
  LCD_WR_DATA(0x37);
  LCD_WR_DATA(0x3c);
  LCD_WR_DATA(0x0F);
  LCD_WR_REG(0x11);

  delay(80);

  // Sleep Out (11h)
  LCD_WR_REG(0x11);
  delay(60);

  //Display ON (29h)
  LCD_WR_REG(0x29);
}

void LCD_Update16(uint16_t *rp, int y, int h) {
  AddressSet(0, y, LCD_WIDTH - 1, y + h - 1);
  LCD_RS_1;
  for (int b = 0; b < h * LCD_WIDTH; b++) {
    LCD_WR_BUS(*rp++);
  }
}

