#include "ssd1306.h"

// Default constructor.
pSSD1306::pSSD1306() {}

// Basic SSD1306 constructor. It accepts a W/H, but currently
// only a resolution of 128x64 is supported.
pSSD1306::pSSD1306(pI2C* I2Cx, uint8_t addr, int display_w, int display_h) {
  i2c = I2Cx;
  address = addr;
  oled_w = display_w;
  oled_h = display_h;
  // Initialize the framebuffer to 0'x.
  int fb_i;
  for (fb_i = 0; fb_i < OLED_MAX_FB_SIZE; ++fb_i) {
    framebuffer[fb_i] = 0x00;
  }
}

/* Drawing methods for the OLED framebuffer. */
/*
 * Draw a horizontal line.
 * First, calculate the Y bitmask and byte offset, then
 * just go across the row from x->x.
 */
void pSSD1306::draw_h_line(int x, int y,
                           int w, unsigned char color) {
  if (x+w > oled_w || x < 0 || y < 0 || y > oled_h) { return; }
  int y_page_offset  = y / 8;
  y_page_offset     *= 128;
  int bit_to_set     = 0x01 << (y & 0x07);
  if (!color) {
    bit_to_set = ~bit_to_set;
  }
  int x_pos;
  for (x_pos = x; x_pos < (x+w); ++x_pos) {
    if (color) {
      framebuffer[x_pos + y_page_offset] |= bit_to_set;
    }
    else {
      framebuffer[x_pos + y_page_offset] &= bit_to_set;
    }
  }
}

/*
 * Draw a veritcal line. The way these displays work in the
 * mode I'm using, they write 8 vertical pixels; the data
 * 'sweeps' across the X coordinates 8 times in a 64px-tall
 * display.
 */
void pSSD1306::draw_v_line(int x, int y,
                           int h, unsigned char color) {
  if (x > oled_w || x < 0 || y < 0 || y+h > oled_h) { return; }
  int y_page_offset;
  int bit_to_set;
  int y_pos;
  for (y_pos = y; y_pos < (y+h); ++y_pos) {
    y_page_offset = y_pos/8;
    y_page_offset *= 128;
    bit_to_set = 0x01 << (y_pos & 0x07);
    if (color) {
      framebuffer[x + y_page_offset] |= bit_to_set;
    }
    else {
      bit_to_set = ~bit_to_set;
      framebuffer[x + y_page_offset] &= bit_to_set;
    }
  }
}

/*
 * Draw a rectangle on the display.
 * I guess just pick the longer dimension, and either draw
 * horizontal or vertical lines.
 * Notable args:
 *   - outline: If <=0, fill the rectangle with 'color'.
 *     If >0, draw an outline inside the dimensions of N pixels.
 *   - color: If 0, clear drawn bits. If not 0, set drawn bits.
 */
void pSSD1306::draw_rect(int x, int y, int w, int h,
                         int outline, unsigned char color) {
  if (x+w > oled_w || x < 0 || y < 0 || y+h > oled_h) { return; }
  if (outline > 0) {
    // Draw an outline.
    int o_pos;
    // Top.
    for (o_pos = y; o_pos < (y+outline); ++o_pos) {
      draw_h_line(x, o_pos, w, color);
    }
    // Bottom.
    for (o_pos = (y+h-1); o_pos > (y+h-1-outline); --o_pos) {
      draw_h_line(x, o_pos, w, color);
    }
    // Left.
    for (o_pos = x; o_pos < (x+outline); ++o_pos) {
      draw_v_line(o_pos, y, h, color);
    }
    // Right.
    for (o_pos = (x+w-1); o_pos > (x+w-1-outline); --o_pos) {
      draw_v_line(o_pos, y, h, color);
    }
  }
  else {
    // Draw a filled rectangle.
    if (w > h) {
      // Draw fewer horizontal lines than vertical ones.
      int y_pos;
      for (y_pos = y; y_pos < (y+h); ++y_pos) {
        draw_h_line(x, y_pos, w, color);
      }
    }
    else {
      // Draw fewer (or ==) vertical lines than horizontal ones.
      int x_pos;
      for (x_pos = x; x_pos < (x+w); ++x_pos) {
        draw_v_line(x_pos, y, h, color);
      }
    }
  }
}

/*
 * Write a pixel in the current OLED framebuffer.
 * Note that the positioning is a bit odd; each byte is a
 * vertical column of 8 pixels, but each successive byte
 * increments the row position by 1. This means that the buffer
 * is 8x 128-byte pages stacked on top of one another. So to
 * set an (x, y) pixel, we |= one position in one byte.
 *   Byte offset = x + ((y / 8) * 128)
 *   Bit offset  = (y & 0x07)
 * 'color' indicates whether to set or unset the pixel.
 * '0' means 'pixel off', non-zero means 'pixel on'.
 */
void pSSD1306::draw_pixel(int x, int y, unsigned char color) {
  // I'm sure the compiler will optimize this away,
  // so I'll try to make the math self-documenting.
  int y_page = y / 8;
  int byte_to_mod = x + (y_page * 128);
  int bit_to_set = 0x01 << (y & 0x07);
  if (color) {
    framebuffer[byte_to_mod] |= bit_to_set;
  }
  else {
    bit_to_set = ~bit_to_set;
    framebuffer[byte_to_mod] &= bit_to_set;
  }
}

/*
 * Draw a letter to the framebuffer, using a 'font' defined
 * in the header file. The font comes in the form of 48-bit
 * glyphs, so this function accepts the two relevant 32-bit
 * words and draws them to the framebuffer.
 */
void pSSD1306::draw_letter(int x, int y,
                           uint32_t w0, uint32_t w1,
                           unsigned char color, char size) {
  // TODO: Comment this method, ffs...
  int w_iter = 0;
  int cur_x = x;
  int cur_y = y;
  uint32_t aw0 = w0;
  uint32_t aw1 = w1;
  if (!color) {
    aw0 = ~aw0;
    aw1 = ~aw1;
  }
  int px_incr = 1;
  int line_h = 8;
  unsigned char t_col = 0x00;
  int cx = cur_x;
  int cy = cur_y;
  if (size == 'L') {
    px_incr = 2;
    line_h = 16;
  }
  for (w_iter = 31; w_iter >= 0; --w_iter) {
    t_col = !(!(aw0 & (1 << w_iter)));
    for (cx = cur_x; cx < cur_x + px_incr; ++cx) {
      for (cy = cur_y; cy < cur_y + px_incr; ++cy) {
        draw_pixel(cx, cy, t_col);
      }
    }
    cur_y += px_incr;
    if (cur_y == y+line_h) {
      cur_y = y;
      cur_x += px_incr;
    }
  }
  for (w_iter = 15; w_iter >= 0; --w_iter) {
    t_col = !(!(aw1 & (1 << w_iter)));
    for (cx = cur_x; cx < cur_x + px_incr; ++cx) {
      for (cy = cur_y; cy < cur_y + px_incr; ++cy) {
        draw_pixel(cx, cy, t_col);
      }
    }
    cur_y += px_incr;
    if (cur_y == y+line_h) {
      cur_y = y;
      cur_x += px_incr;
    }
  }
}

/*
 * Draw a single character to the OLED display.
 * This method translates between a character and the
 * font's pixel data defined in the header file.
 */
void pSSD1306::draw_letter_c(int x, int y, char c,
                             unsigned char color, char size) {
  unsigned int w0 = 0x00;
  unsigned int w1 = 0x00;
  if (c == 'A') {
    w0 = OLED_CH_A0;
    w1 = OLED_CH_A1B1 >> 16;
  }
  else if (c == 'B') {
    w0 = OLED_CH_B0;
    w1 = OLED_CH_A1B1 & 0x0000FFFF;
  }
  else if (c == 'C') {
    w0 = OLED_CH_C0;
    w1 = OLED_CH_C1D1 >> 16;
  }
  else if (c == 'D') {
    w0 = OLED_CH_D0;
    w1 = OLED_CH_C1D1 & 0x0000FFFF;
  }
  else if (c == 'E') {
    w0 = OLED_CH_E0;
    w1 = OLED_CH_E1F1 >> 16;
  }
  else if (c == 'F') {
    w0 = OLED_CH_F0;
    w1 = OLED_CH_E1F1 & 0x0000FFFF;
  }
  else if (c == 'G') {
    w0 = OLED_CH_G0;
    w1 = OLED_CH_G1H1 >> 16;
  }
  else if (c == 'H') {
    w0 = OLED_CH_H0;
    w1 = OLED_CH_G1H1 & 0x0000FFFF;
  }
  else if (c == 'I') {
    w0 = OLED_CH_I0;
    w1 = OLED_CH_I1J1 >> 16;
  }
  else if (c == 'J') {
    w0 = OLED_CH_J0;
    w1 = OLED_CH_I1J1 & 0x0000FFFF;
  }
  else if (c == 'K') {
    w0 = OLED_CH_K0;
    w1 = OLED_CH_K1L1 >> 16;
  }
  else if (c == 'L') {
    w0 = OLED_CH_L0;
    w1 = OLED_CH_K1L1 & 0x0000FFFF;
  }
  else if (c == 'M') {
    w0 = OLED_CH_M0;
    w1 = OLED_CH_M1N1 >> 16;
  }
  else if (c == 'N') {
    w0 = OLED_CH_N0;
    w1 = OLED_CH_M1N1 & 0x0000FFFF;
  }
  else if (c == 'O') {
    w0 = OLED_CH_O0;
    w1 = OLED_CH_O1P1 >> 16;
  }
  else if (c == 'P') {
    w0 = OLED_CH_P0;
    w1 = OLED_CH_O1P1 & 0x0000FFFF;
  }
  else if (c == 'Q') {
    w0 = OLED_CH_Q0;
    w1 = OLED_CH_Q1R1 >> 16;
  }
  else if (c == 'R') {
    w0 = OLED_CH_R0;
    w1 = OLED_CH_Q1R1 & 0x0000FFFF;
  }
  else if (c == 'S') {
    w0 = OLED_CH_S0;
    w1 = OLED_CH_S1T1 >> 16;
  }
  else if (c == 'T') {
    w0 = OLED_CH_T0;
    w1 = OLED_CH_S1T1 & 0x0000FFFF;
  }
  else if (c == 'U') {
    w0 = OLED_CH_U0;
    w1 = OLED_CH_U1V1 >> 16;
  }
  else if (c == 'V') {
    w0 = OLED_CH_V0;
    w1 = OLED_CH_U1V1 & 0x0000FFFF;
  }
  else if (c == 'W') {
    w0 = OLED_CH_W0;
    w1 = OLED_CH_W1X1 >> 16;
  }
  else if (c == 'X') {
    w0 = OLED_CH_X0;
    w1 = OLED_CH_W1X1 & 0x0000FFFF;
  }
  else if (c == 'Y') {
    w0 = OLED_CH_Y0;
    w1 = OLED_CH_Y1Z1 >> 16;
  }
  else if (c == 'Z') {
    w0 = OLED_CH_Z0;
    w1 = OLED_CH_Y1Z1 & 0x0000FFFF;
  }
  else if (c == 'a') {
    w0 = OLED_CH_a0;
    w1 = OLED_CH_a1b1 >> 16;
  }
  else if (c == 'b') {
    w0 = OLED_CH_b0;
    w1 = OLED_CH_a1b1 & 0x0000FFFF;
  }
  else if (c == 'c') {
    w0 = OLED_CH_c0;
    w1 = OLED_CH_c1d1 >> 16;
  }
  else if (c == 'd') {
    w0 = OLED_CH_d0;
    w1 = OLED_CH_c1d1 & 0x0000FFFF;
  }
  else if (c == 'e') {
    w0 = OLED_CH_e0;
    w1 = OLED_CH_e1f1 >> 16;
  }
  else if (c == 'f') {
    w0 = OLED_CH_f0;
    w1 = OLED_CH_e1f1 & 0x0000FFFF;
  }
  else if (c == 'g') {
    w0 = OLED_CH_g0;
    w1 = OLED_CH_g1h1 >> 16;
  }
  else if (c == 'h') {
    w0 = OLED_CH_h0;
    w1 = OLED_CH_g1h1 & 0x0000FFFF;
  }
  else if (c == 'i') {
    w0 = OLED_CH_i0;
    w1 = OLED_CH_i1j1 >> 16;
  }
  else if (c == 'j') {
    w0 = OLED_CH_j0;
    w1 = OLED_CH_i1j1 & 0x0000FFFF;
  }
  else if (c == 'k') {
    w0 = OLED_CH_k0;
    w1 = OLED_CH_k1l1 >> 16;
  }
  else if (c == 'l') {
    w0 = OLED_CH_l0;
    w1 = OLED_CH_k1l1 & 0x0000FFFF;
  }
  else if (c == 'm') {
    w0 = OLED_CH_m0;
    w1 = OLED_CH_m1n1 >> 16;
  }
  else if (c == 'n') {
    w0 = OLED_CH_n0;
    w1 = OLED_CH_m1n1 & 0x0000FFFF;
  }
  else if (c == 'o') {
    w0 = OLED_CH_o0;
    w1 = OLED_CH_o1p1 >> 16;
  }
  else if (c == 'p') {
    w0 = OLED_CH_p0;
    w1 = OLED_CH_o1p1 & 0x0000FFFF;
  }
  else if (c == 'q') {
    w0 = OLED_CH_q0;
    w1 = OLED_CH_q1r1 >> 16;
  }
  else if (c == 'r') {
    w0 = OLED_CH_r0;
    w1 = OLED_CH_q1r1 & 0x0000FFFF;
  }
  else if (c == 's') {
    w0 = OLED_CH_s0;
    w1 = OLED_CH_s1t1 >> 16;
  }
  else if (c == 't') {
    w0 = OLED_CH_t0;
    w1 = OLED_CH_s1t1 & 0x0000FFFF;
  }
  else if (c == 'u') {
    w0 = OLED_CH_u0;
    w1 = OLED_CH_u1v1 >> 16;
  }
  else if (c == 'v') {
    w0 = OLED_CH_v0;
    w1 = OLED_CH_u1v1 & 0x0000FFFF;
  }
  else if (c == 'w') {
    w0 = OLED_CH_w0;
    w1 = OLED_CH_w1x1 >> 16;
  }
  else if (c == 'x') {
    w0 = OLED_CH_x0;
    w1 = OLED_CH_w1x1 & 0x0000FFFF;
  }
  else if (c == 'y') {
    w0 = OLED_CH_y0;
    w1 = OLED_CH_y1z1 >> 16;
  }
  else if (c == 'z') {
    w0 = OLED_CH_z0;
    w1 = OLED_CH_y1z1 & 0x0000FFFF;
  }
  else if (c == '0') {
    w0 = OLED_CH_00;
    w1 = OLED_CH_0111 >> 16;
  }
  else if (c == '1') {
    w0 = OLED_CH_10;
    w1 = OLED_CH_0111 & 0x0000FFFF;
  }
  else if (c == '2') {
    w0 = OLED_CH_20;
    w1 = OLED_CH_2131 >> 16;
  }
  else if (c == '3') {
    w0 = OLED_CH_30;
    w1 = OLED_CH_2131 & 0x0000FFFF;
  }
  else if (c == '4') {
    w0 = OLED_CH_40;
    w1 = OLED_CH_4151 >> 16;
  }
  else if (c == '5') {
    w0 = OLED_CH_50;
    w1 = OLED_CH_4151 & 0x0000FFFF;
  }
  else if (c == '6') {
    w0 = OLED_CH_60;
    w1 = OLED_CH_6171 >> 16;
  }
  else if (c == '7') {
    w0 = OLED_CH_70;
    w1 = OLED_CH_6171 & 0x0000FFFF;
  }
  else if (c == '8') {
    w0 = OLED_CH_80;
    w1 = OLED_CH_8191 >> 16;
  }
  else if (c == '9') {
    w0 = OLED_CH_90;
    w1 = OLED_CH_8191 & 0x0000FFFF;
  }
  else if (c == ':') {
    w0 = OLED_CH_col0;
    w1 = OLED_CH_col1per1 >> 16;
  }
  else if (c == '.') {
    w0 = OLED_CH_per0;
    w1 = OLED_CH_col1per1 & 0x0000FFFF;
  }
  else if (c == '!') {
    w0 = OLED_CH_exc0;
    w1 = OLED_CH_exc1fws1 >> 16;
  }
  else if (c == '/') {
    w0 = OLED_CH_fws0;
    w1 = OLED_CH_exc1fws1 & 0x0000FFFF;
  }
  else if (c == '-') {
    w0 = OLED_CH_hyp0;
    w1 = OLED_CH_hyp1pls1 >> 16;
  }
  else if (c == '+') {
    w0 = OLED_CH_pls0;
    w1 = OLED_CH_hyp1pls1 & 0x0000FFFF;
  }
  else if (c == '<') {
    w0 = OLED_CH_lct0;
    w1 = OLED_CH_lct1rct1 >> 16;
  }
  else if (c == '>') {
    w0 = OLED_CH_rct0;
    w1 = OLED_CH_lct1rct1 & 0x0000FFFF;
  }
  // Draw the glyph to the framebuffer.
  // (If the character was not found, an empty space is drawn.)
  draw_letter(x, y, w0, w1, color, size);
}

/*
 * Draw an integer to the display.
 * Take modulus values with decreasing powers of 10 and print
 * the digits one-by-one. Assumes the int is <= 9,999,999,999
 */
void pSSD1306::draw_letter_i(int x, int y, int ic,
                             unsigned char color, char size) {
  int magnitude = 1000000000;
  int cur_x = x;
  int first_found = 0;
  int proc_val = ic;
  if (proc_val < 0) {
    proc_val = proc_val * -1;
    draw_letter_c(cur_x, y, '-', color, size);
    if (size == 'S') {
      cur_x += 6;
    }
    else if (size == 'L') {
      cur_x += 12;
    }
  }
  for (magnitude = 1000000000; magnitude > 0; magnitude /= 10) {
    int m_val = proc_val / magnitude;
    proc_val -= (m_val * magnitude);
    if (m_val > 0 || first_found || magnitude == 1) {
      first_found = 1;
      char mc = ' ';
      if (m_val == 0) {
        mc = '0';
      }
      else if (m_val == 1) {
        mc = '1';
      }
      else if (m_val == 2) {
        mc = '2';
      }
      else if (m_val == 3) {
        mc = '3';
      }
      else if (m_val == 4) {
        mc = '4';
      }
      else if (m_val == 5) {
        mc = '5';
      }
      else if (m_val == 6) {
        mc = '6';
      }
      else if (m_val == 7) {
        mc = '7';
      }
      else if (m_val == 8) {
        mc = '8';
      }
      else if (m_val == 9) {
        mc = '9';
      }
      draw_letter_c(cur_x, y, mc, color, size);
      if (size == 'S') {
        cur_x += 6;
      }
      else if (size == 'L') {
        cur_x += 12;
      }
      if (cur_x >= 128) { return; }
    }
  }
}

/*
 * Draw a string of text.
 * Careful; this assumes the text is a C-string ending in '\0'.
 */
void pSSD1306::draw_text(int x, int y, const char* cc,
                         unsigned char color, const char size) {
  int i = 0;
  int offset = 0;
  while (cc[i] != '\0') {
    draw_letter_c(x + offset, y, cc[i], color, size);
    if (size == 'S') {
      offset += 6;
    }
    else if (size == 'L') {
      offset += 12;
    }
    ++i;
  }
}

/*
 * Write a 'command byte' to the display.
 * Sending 0x00 as a first byte indicates a command.
 * TODO: Support alternate address of 0x7A.
 */
void pSSD1306::write_command_byte(uint8_t cmd) {
  #if    defined(STARm_F3)
    i2c->set_num_bytes(2);
    i2c->start(0x78);
    i2c->write(0x00);
    i2c->write(cmd);
    i2c->stop();
  #elif  STARm_F1
    i2c->start(0x78);
    i2c->write(0x00);
    i2c->write(cmd);
    i2c->stop();
  #endif
}

/*
 * Write a 'data byte' to the display.
 * Sending 0x40 as a first byte indicates that
 * display data will follow.
 * TODO: Support alternate address of 0x7A.
 */
void pSSD1306::write_data_byte(uint8_t dat) {
  #if    defined(STARm_F3)
    i2c->set_num_bytes(2);
    i2c->start(0x78);
    i2c->write(0x40);
    i2c->write(dat);
    i2c->stop();
  #elif  STARm_F1
    i2c->start(0x78);
    i2c->write(0x40);
    i2c->write(dat);
    i2c->stop();
  #endif
}

/*
 * Initialize a 128x64-pixel SSD1306 display.
 * TODO: Support resolutions other than 128x64
 */
void pSSD1306::init_display(void) {
  // Display clock division
  write_command_byte(0xD5);
  write_command_byte(0x80);
  // Set multiplex
  write_command_byte(0xA8);
  write_command_byte(0x3F);
  // Set display offset ('start column')
  write_command_byte(0xD3);
  write_command_byte(0x00);
  // Set start line (0b01000000 | line)
  write_command_byte(0x40);
  // Set internal charge pump (on)
  write_command_byte(0x8D);
  write_command_byte(0x14);
  // Set memory mode
  write_command_byte(0x20);
  write_command_byte(0x00);
  // Set 'SEGREMAP'
  write_command_byte(0xA1);
  // Set column scan (descending)
  write_command_byte(0xC8);
  // Set 'COMPINS'
  write_command_byte(0xDA);
  write_command_byte(0x12);
  // Set contrast
  write_command_byte(0x81);
  write_command_byte(0xCF);
  // Set precharge
  write_command_byte(0xD9);
  write_command_byte(0xF1);
  // Set VCOM detect
  write_command_byte(0xDB);
  write_command_byte(0x40);
  // Set output to follow RAM content
  write_command_byte(0xA4);
  // Normal display mode
  write_command_byte(0xA6);
  // Display on
  write_command_byte(0xAF);
}

/*
 * Draw the entire framebuffer to a 128x64-pixel SSD1306 display.
 * TODO: Support resolutions other than 128x64.
 */
void pSSD1306::draw_framebuffer() {
  #if    defined(STARm_F3)
    // Set the 'RELOAD' flag.
    i2c->set_reload_flag(1);
    // Set the device address and send a 'start' condition.
    i2c->set_num_bytes(1);
    i2c->start(0x78);
    // Send one byte to set a 'data' transmission.
    i2c->write(0x40);
    // Send the rest of the framebuffer.
    i2c->stream((volatile void*)framebuffer, (oled_w*oled_h)/8);
    // Send a 'stop' condition.
    i2c->stop();
  #elif  STARm_F1
    // Start with the display's address.
    i2c->start(0x78);
    // Set a 'data' transmission.
    i2c->write(0x40);
    // Stream the framebuffer.
    i2c->stream((volatile void*)framebuffer, (oled_w*oled_h)/8);
    // Send a 'stop' condition.
    i2c->stop();
  #endif
}
