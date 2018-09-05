#ifndef __STARm_PERIPHS_H
#define __STARm_PERIPHS_H

// Project includes.
#include "core.h"
#include "i2c.h"

// SSD1306 device declarations.
// Buffer for the OLED screen.
// Currently only supports 128x64-px monochrome.
// (1 Byte = 8 pixels)
#define OLED_FB_SIZE (128*64)/8
extern volatile uint8_t oled_fb[OLED_FB_SIZE];
// Buffer for drawing lines of text to the OLED.
//static char oled_line_buf[24];

/*
 * SSD1306 device class.
 * TODO: Support resolutions other than 128x64.
 */
#define OLED_MAX_FB_SIZE ((128*64)/8)
class pSSD1306 {
public:
  // Constructors.
  pSSD1306();
  pSSD1306(pI2C* I2Cx, uint8_t addr,
           int display_w, int display_h);
  // Main display methods.
  void init_display(void);
  void draw_framebuffer(void);
  // Drawing methods.
  // These write to the framebuffer and don't draw to the display.
  void draw_h_line(int x, int y, int w, unsigned char color);
  void draw_v_line(int x, int y, int h, unsigned char color);
  void draw_rect(int x, int y, int w, int h,
                 int outline, unsigned char color);
  void draw_pixel(int x, int y, unsigned char color);
  void draw_letter(int x, int y, uint32_t w0, uint32_t w1,
                   unsigned char color, char size);
  void draw_letter_c(int x, int y, char c,
                     unsigned char color, char size);
  void draw_letter_i(int x, int y, int ic,
                     unsigned char color, char size);
  void draw_text(int x, int y, const char* cc,
                 unsigned char color, const char size);
  // Getters/Setters.
  int get_status(void);

  // Basic properties.
  int     oled_w;
  int     oled_h;
  uint8_t address;
protected:
  // I2C peripheral interface.
  pI2C*  i2c;
  // Expected status.
  int status = pSTATUS_ERR;
  // TODO: Better way of sizing the framebuffer.
  volatile uint8_t framebuffer[OLED_MAX_FB_SIZE];

  void write_command_byte(uint8_t cmd);
  void write_data_byte(uint8_t dat);
private:
};

// Define a simple monospace font; each character is 6x8 pixels,
// which comes out to 6 bytes or 3 words for every 2 characters.
#define OLED_CH_A0       0x1F688868
#define OLED_CH_B0       0xFF898989
#define OLED_CH_A1B1     0x1F007600
#define OLED_CH_C0       0x7E818181
#define OLED_CH_D0       0xFF818181
#define OLED_CH_C1D1     0x66007E00
#define OLED_CH_E0       0xFF898989
#define OLED_CH_F0       0xFF888888
#define OLED_CH_E1F1     0x81008000
#define OLED_CH_G0       0x7E818989
#define OLED_CH_H0       0xFF080808
#define OLED_CH_G1H1     0x6E00FF00
#define OLED_CH_I0       0x8181FF81
#define OLED_CH_J0       0x868181FE
#define OLED_CH_I1J1     0x81008000
#define OLED_CH_K0       0xFF182442
#define OLED_CH_L0       0xFF010101
#define OLED_CH_K1L1     0x81000100
#define OLED_CH_M0       0xFF403040
#define OLED_CH_N0       0xFF601806
#define OLED_CH_M1N1     0xFF00FF00
#define OLED_CH_O0       0x7E818181
#define OLED_CH_P0       0xFF888888
#define OLED_CH_O1P1     0x7E007000
#define OLED_CH_Q0       0x7E818582
#define OLED_CH_R0       0xFF888C8A
#define OLED_CH_Q1R1     0x7D007100
#define OLED_CH_S0       0x66919989
#define OLED_CH_T0       0x8080FF80
#define OLED_CH_S1T1     0x66008000
#define OLED_CH_U0       0xFE010101
#define OLED_CH_V0       0x701C031C
#define OLED_CH_U1V1     0xFE00E000
#define OLED_CH_W0       0xFE010601
#define OLED_CH_X0       0xC3241824
#define OLED_CH_W1X1     0xFE00C300
#define OLED_CH_Y0       0xE0100F10
#define OLED_CH_Z0       0x838599A1
#define OLED_CH_Y1Z1     0xE000C100
#define OLED_CH_a0       0x06292929
#define OLED_CH_b0       0xFF090909
#define OLED_CH_a1b1     0x1F000600
#define OLED_CH_c0       0x1E212121
#define OLED_CH_d0       0x060909FF
#define OLED_CH_c1d1     0x12000100
#define OLED_CH_e0       0x3E494949
#define OLED_CH_f0       0x087F8888
#define OLED_CH_e1f1     0x3A006000
#define OLED_CH_g0       0x32494949
#define OLED_CH_h0       0xFF080808
#define OLED_CH_g1h1     0x3E000700
#define OLED_CH_i0       0x00004F00
#define OLED_CH_j0       0x0006015E
#define OLED_CH_i1j1     0x00000000
#define OLED_CH_k0       0x00FF1C23
#define OLED_CH_l0       0x0000FF00
#define OLED_CH_k1l1     0x00000000
#define OLED_CH_m0       0x3F101F10
#define OLED_CH_n0       0x3F10100F
#define OLED_CH_m1n1     0x0F000000
#define OLED_CH_o0       0x0E111111
#define OLED_CH_p0       0x003F2424
#define OLED_CH_o1p1     0x0E001800
#define OLED_CH_q0       0x3048487E
#define OLED_CH_r0       0x003F1010
#define OLED_CH_q1r1     0x01000800
#define OLED_CH_s0       0x00324949
#define OLED_CH_t0       0x20FE2121
#define OLED_CH_s1t1     0x26000200
#define OLED_CH_u0       0x3C02023E
#define OLED_CH_v0       0x18060106
#define OLED_CH_u1v1     0x03001800
#define OLED_CH_w0       0x1E010201
#define OLED_CH_x0       0x110A040A
#define OLED_CH_w1x1     0x1E001100
#define OLED_CH_y0       0x3209093E
#define OLED_CH_z0       0x11131519
#define OLED_CH_y1z1     0x00001100
#define OLED_CH_00       0x7EE19987
#define OLED_CH_10       0x2141FF01
#define OLED_CH_0111     0x7E000100
#define OLED_CH_20       0x63878D99
#define OLED_CH_30       0x66818989
#define OLED_CH_2131     0x71007600
#define OLED_CH_40       0xF80808FF
#define OLED_CH_50       0xE2919191
#define OLED_CH_4151     0x08008E00
#define OLED_CH_60       0x7E919191
#define OLED_CH_70       0x60838CB0
#define OLED_CH_6171     0x4E00C000
#define OLED_CH_80       0x6E919191
#define OLED_CH_90       0x72898989
#define OLED_CH_8191     0x6E007E00
#define OLED_CH_col0     0x00002400
#define OLED_CH_per0     0x00000002
#define OLED_CH_col1per1 0x00000000
#define OLED_CH_exc0     0x007A0000
#define OLED_CH_fws0     0x00061860
#define OLED_CH_exc1fws1 0x00000000
#define OLED_CH_hyp0     0x00080808
#define OLED_CH_pls0     0x00081C08
#define OLED_CH_hyp1pls1 0x00000000
#define OLED_CH_lct0     0x00081422
#define OLED_CH_rct0     0x00442810
#define OLED_CH_lct1rct1 0x00000000

#endif
