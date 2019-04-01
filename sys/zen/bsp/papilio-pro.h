#ifndef __ZEN_BSP_PAPILIO_PRO_H__
#define __ZEN_BSP_PAPILIO_PRO_H__

#if defined(ZEN_PAPILIO_PRO)

#include <stdint.h>

#define V0_PIX_MASK_BIT (1 << 0)
#define V0_PIX_BUF_BIT  (1 << 1)
#define V0_PIX_SHIFT    4
#define V0_PIX_MASK     0x0f

#define V0_FPGA_RESET   37
#define V0_LED1_OUT     112

typedef int16_t         v0pix12;
typedef v0pix12         v0pixel;

#define v0getblue(ptr)  ((uint8_t *)(ptr)[0] & V0_PIX_MASK)
#define v0getgreen(ptr) (((uint8_t *)(ptr)[0] >> V0_PIX_SHIFT) & V0_PIX_MASK)
#define V0getred(ptr)   ((uint8_t *)(ptr)[1] & V0_PIX_MASK)
#define v0getflg(ptr)   (((uint8_t *)(ptr)[1] >> V0_PIX_SHIFT) & V0_PIX_MASK)

#define V0_TIMER_PREC   32000000                // 32 MHz oscillator
#define V0_TIMER_MUL    2                       // 64 MHz system bus
#define V0_TIMER_HZ     64000000.0              // system timer frequency
#define V0_HZ           100                     // kernel timer interrupt speed
#define V0_HZ_MUL       (long)(V0_TIMER_HZ / V0_HZ) // timer frequency factor
#define V0_MIN_TICKS    1                       // BVT-scheduler parameter
#define V0_FLASH_SIZE   (8U * 1024 * 1024)      // size of SPI flash memory
#define V0_RAM_SIZE     (8U * 1024 * 1024)      // size of SDRAM
#define V0_VIDEO_WIDTH  640                     // horizontal video resolution
#define V0_VIDEO_HEIGHT 480                     // vertical resolution
#define V0_VIDEO_BUFSZ  (V0_VIDEO_WIDTH * V0_VIDEO_HEIGHT * sizeof(v0pixel))
#define V0_VGA_DEPTH    4096
#define V0_VGA_BITS     12
#define V0_BRAM_FB      0x00000000
#define V0_FB_SIZE      460800
#define V0_CONF_SIZE    65536
#define V0_CONF_BRAM    (V0_BRAM_SIZE - V0_CONF_SIZE - V0_FB_SIZE)
#define V0_BRAM_UNIT    18000
#define V0_BRAM_UNITS   32
#define V0_BRAM_SIZE    (V0_BRAM_UNIT * V0_BRAM_UNITS)
#define V0_SRAM_SIZE    65536

/* device flags */
#define V0_DAC_BIT      (1 << 0)
#define V0_PS2_KBD      (1 << 1)
#define V0_PS2_MOUSE    (1 << 2)
#define V0_PS2_JSTICK1  (1 << 3)
#define V0_PS2_JSTICK2  (1 << 4)
#define V0_UP_BUTTON    (1 << 5)
#define V0_DOWN_BUTTON  (1 << 6)
#define V0_LEFT_BUTTON  (1 << 7)
#define V0_RIGHT_BUTTON (1 << 8)
#define V0_LED1         (1 << 9)
#define V0_LED2         (1 << 10)
#define V0_LED3         (1 << 11)
#define V0_LED4         (1 << 12)

#define V0_IO_PINS      48
#define V0_JTAG_TMS_IN  107
#define V0_JTAG_TCK_IN  109
#define V0_JTAG_SI_IN   64
#define V0_JTAG_SO_IN   65

/* USB pins */
#define V0_SER_RX_IN    101     // serial receive
#define V0_SER_TX_OUT   105     // serial transmit
#define V0_FLASH_CS     38      // chip select
#define V0_FLASH_CLK    70      // clock
#define V0_FLASH_MOSI   64      // SPI flash master out slave iin
#define V0_FLASH_MISO   65      // SPI flast master in slave out

/* oscillator pin */
#define V0_TIMER        94

/* VGA pinout */
#define V0_VGA_RED0     118
#define V0_VGA_RED2     119
#define V0_VGA_RED3     120
#define V0_VGA_RED4     121
#define V0_VGA_GREEN0   84
#define V0_VGA_GREEN1   83
#define V0_VGA_GREEN2   80
#define V0_VGA_GREEN3   78
#define V0_VGA_BLUE0    99
#define V0_VGA_BLUE1    97
#define V0_VGA_BLUE2    92
#define V0_VGA_BLUE3    87
#define V0_VGA_HSYNC    117
#define V0_VGA_VSYNC    116

/* audio pins */
#define V0_AUDIO_LEFT   98
#define V0_AUDIO_RIGHT  100

/* button pins */
#define V0_LEFT_PIN     74
#define V0_UP_PIN       95
#define V0_RIGHT_PIN    59
#define V0_DOWN_PIN     62

/* led pins */
#define V0_PLAYER1_LED  75
#define V0_PLAYER2_LED  67
#define V0_FLIPPED_LED  66
#define V0_LED4_PIN     61

/* joystick #1 */
#define V0_JOY1_UP      123
#define V0_JOY1_DOWN    126
#define V0_JOY1_LEFT    127
#define V0_JOY1_RIGHT   132
#define V0_JOY1_FIRE2   134
#define V0_JOY1_FIRE1   124
#define V0_JOY1_5VOLTS  0xff
#define V0_JOY1_GROUND  44

/* joystick #2 */
#define V0_JOY2_UP      57
#define V0_JOY2_DOWN    50
#define V0_JOY2_LEFT    47
#define V0_JOY2_RIGHT   51
#define V0_JOY2_FIRE2   58
#define V0_JOY2_FIRE1   55
#define V0_JOY2_5VOLTS  0xff
#define V0_JOY2_GROUND  48

/* keyboard and mouse */
#define V0_PS2_CLK1     115
#define V0_PS2_DATA1    114
#define V0_PS2_CLK2     93
#define V0_PS2_DATA2    88

#define V0_RESET_PIN    85

#endif /* ZEN_PAPILIO_PRO */

#endif __ZEN_BSP_PAPILIO_PRO_H__

