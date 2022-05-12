#ifndef CONSTANTS_H
#define CONSTANTS_H

//TODO: bitfield for macros here or GPIO registers?
//TODO: turn these into constants

//see BCM2835 Peripheral manual
#define BCM2835_PERI_BASE 0x20000000
#define GPIO_BASE         (BCM2835_PERI_BASE + 0x200000)

//GPIO function select registers (offset)
#define GPFSEL0           0x0
#define GPFSEL1           0x4
#define GPFSEL2           0x8

//GPIO set register address (offset)
#define GPSET0            0x1C

//GPIO clear register address (offset)
#define GPCLR0            0x28

//Function select register offsets
#define FSEL2             6
#define FSEL3             9
#define FSEL4             12
#define FSEL9             27
#define FSEL10            0
#define FSEL11            3
#define FSEL17            21
#define FSEL22            6
#define FSEL23            9
#define FSEL24            12
#define FSEL27            21

//GPIO set & clear register offsets for LCD pins
#define RS 2
#define RW 3
#define E  4
#define D0 9
#define D1 10
#define D2 11
#define D3 17
#define D4 22
#define D5 23
#define D6 24
#define D7 27

//Pins
#define LCD_REGISTER_SET_PIN 0b00000000001
#define LCD_READ_WRITE_PIN   0b00000000010
#define LCD_ENABLE_PIN       0b00000000100
#define LCD_D0_PIN           0b00000001000
#define LCD_D1_PIN           0b00000010000
#define LCD_D2_PIN           0b00000100000
#define LCD_D3_PIN           0b00001000000
#define LCD_D4_PIN           0b00010000000
#define LCD_D5_PIN           0b00100000000
#define LCD_D6_PIN           0b01000000000
#define LCD_D7_PIN           0b10000000000
#define LCD_ALL_PINS         0b11111111111

//Instructions
#define LCD_CLEAR_DISPLAY        0b00000001000

#define LCD_RETURN_HOME          0b00000010000

#define LCD_ENTRY_MODE_SET       0b00000100000
#define LCD_INCREMENT            0b00000010000
#define LCD_DECREMENT            0b00000000000
#define LCD_SHIFT_ACCOMPANIES    0b00000001000

#define LCD_DISPLAY_CONTROL      0b00001000000
#define LCD_DISPLAY_ON           0b00000100000
#define LCD_DISPLAY_OFF          0b00000000000
#define LCD_CURSOR_ON            0b00000010000
#define LCD_CURSOR_OFF           0b00000000000
#define LCD_CURSOR_BLINK         0b00000001000
#define LCD_CURSOR_NOBLINK       0b00000000000

#define LCD_CURSOR_DISPLAY_SHIFT 0b00010000000
#define LCD_DISPLAY_SHIFT        0b00001000000
#define LCD_CURSOR_MOVE          0b00000000000
#define LCD_SHIFT_RIGHT          0b00000100000
#define LCD_SHIFT_LEFT           0b00000000000

#define LCD_FUNCTION_SET         0b00100000000
#define LCD_8BIT                 0b00010000000
#define LCD_4BIT                 0b00000000000
#define LCD_2LINE                0b00001000000
#define LCD_1LINE                0b00000000000
#define LCD_5x10                 0b00000100000
#define LCD_5x8                  0b00000000000

#define LCD_SET_DDRAM_ADDRESS    0b10000000000

//uncomment this if you want word-wrap enabled
//word-wrap will automatically move the cursor to the second line if text is written
//outside of the visible area
#define ENABLE_WORD_WRAP

#endif
