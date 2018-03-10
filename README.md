# Shooting game with STM32 and color LCD module

[Playing movies here.](https://youtube.com/watch?v=3D_K6kPLnKg")

## Build environment

Arduino IDE with Arduino_STM32.

See [this site](http://wiki.stm32duino.com/index.php?title=Main_Page).

## Hardware

### Photos

<p>
<img src="https://i.imgur.com/n6X9Nu8.jpg" height="180">
<img src="https://i.imgur.com/n6X9Nu8.jpg" height="180">
<img src="https://i.imgur.com/I1A6tYi.jpg" height="180">
<img src="https://i.imgur.com/Fph37qQ.jpg" height="180">
<img src="https://i.imgur.com/7RsKZad.jpg" height="180">
<img src="https://i.imgur.com/QC5zzEq.jpg" height="180">
<img src="https://i.imgur.com/uCqHb04.jpg" height="180">
</p>

### Parts

|Name|Description|
|----|------|
|LCD|Color LCD module TJC-9341-032. <br> Internal chip:ILI9341.|
|CPU|STM32F103C8T6 CPU Board. <br> I do not know if it completely matches the board called 'Blue Pill', <br> but I think it's pretty close.|
|SW_FIRE|Push button|
|SW_RIGHT|Push button|
|SW_LEFT|Push button|
|VREG|Series regulator(5V to 3.3V)|
|R1|USB D+ pullup register, 3.3K Ohm <br> 3.3K is not an ideal value, I just used the thing in the tool box.|
|C1|Polarized capacitor 47uF|
|C2|Polarized capacitor 47uF|
|C3|Ceramic capacitor 0.1uF|

### Circuit

<p>
<img src="https://i.imgur.com/HOTCeMI.png" height="320">
</p>

Regulator generates 3.3V from USB +5V.

LCD board operates at 3.3V. (Signal line is also 3.3V level)

|Connect from|Connect to|
|----------|--------|
|LCD:GND|GND|
|LCD:VCC|3.3V|
|LCD:LED_A|3.3V|
|||
|CPU:PB12|LCD:DB12|
|CPU:PB13|LCD:DB13|
|CPU:PB14|LCD:DB14|
|CPU:PB15|LCD:DB15|
|CPU:PA8|(NC)|
|CPU:PA9|(NC)|
|CPU:PA10|(NC)|
|CPU:PA11|(NC)|
|CPU:PA12|R1:1|
|CPU:PA15|(NC)|
|CPU:PB3|SW_FIRE:1|
|CPU:PB4|(NC)|
|CPU:PB5|LCD:RS|
|CPU:PB6|LCD:WR|
|CPU:PB7|LCD:RD|
|CPU:PB8|LCD:DB8|
|CPU:PB9|LCD:DB9|
|CPU:5V|5V|
|CPU:GND|GND|
|CPU:3V3|(NC)|
|||
|CPU:GND|GND|
|CPU:GND|GND|
|CPU:3V3|(NC)|
|CPU:NRST|(NC)|
|CPU:PB11|LCD:DB11|
|CPU:PB10|LCD:DB10|
|CPU:PB1|SW_RIGHT:1|
|CPU:PB0|SW_LEFT:1|
|CPU:PA7|LCD:DB7|
|CPU:PA6|LCD:DB6|
|CPU:PA5|LCD:DB5|
|CPU:PA4|LCD:DB4|
|CPU:PA3|LCD:DB3|
|CPU:PA2|LCD:DB2|
|CPU:PA1|LCD:DB1|
|CPU:PA0|LCD:DB0|
|CPU:PC15|(NC)|
|CPU:PC14|LCD:CS|
|CPU:PC13|LCD:REST|
|CPU:VBAT|(NC)|
|||
|R1:2|+5V|
|||
|SW_FIRE:2|GND|
|SW_RIGHT:2|GND|
|SW_LEFT:2|GND|
|||
|VREG:IN|5V|
|VREG:OUT|3.3V|
|VREG:GND|GND|
|C1:+|VREG:IN|
|C1:-|VREG:GND|
|C2:+|VREG:OUT|
|C2:-|VREG:GND|
|C3:1|VREG:OUT|
|C3:2|VREG:GND|
