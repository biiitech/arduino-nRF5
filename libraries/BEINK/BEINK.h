///
/// @file BEINK.h
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#ifndef BEINK_h
#define BEINK_h

#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>
#include "fonts.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

// Display resolution
#define EPD_WIDTH      104
#define EPD_HEIGHT     212
#define QRCODE_VERSION 2

//#define EPD_WIDTH       128
//#define EPD_HEIGHT      296

// commands
#define DRIVER_OUTPUT          0x01
#define SOURCE_DRIVING_VOLTAGE 0x04
#define DEEP_SLEEP             0x10
#define DATA_ENTRY_SEQUENCE    0x11
#define SW_RESET               0x12

#define DISPLAY_UPDATE_SEQUENCE_RUN 0x20
#define DISPLAY_UPDATE_CTRL         0x21
#define DISPLAY_UPDATE_SEQUENCE_CFG 0x22
#define BW_RAM_ADDRESS              0x24
#define RED_RAM_ADDRESS             0x26
#define WRITE_VCOM_REGISTER         0x2C
#define WRITE_LUT_REGISTER          0x32
#define DUMMY_LINE_PERIOD           0x3A
#define GATE_LINE_WIDTH             0x3B
#define BORDER_WAVEFORM             0x3C
#define RAM_X_START_END             0x44
#define RAM_Y_START_END             0x45
#define RAM_X_COUNTER               0x4E
#define RAM_Y_COUNTER               0x4F
#define ANALOG_BLOCK                0x74
#define DIGITAL_BLOCK               0x7E

#define COLOR_BW  0x01
#define COLOR_RED 0x02

enum eink_color_id {
    color_white = 0,
    color_red = 1,
    color_black = 2,
};

enum eink_orientation {
    ROTATE_0,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270,
};

class BEINKClass {
private:
    int width = EPD_WIDTH;
    int height = EPD_HEIGHT;
    int busy_pin = E_INK_BUSY_PIN;
    int vcc_pin = E_INK_VDD_CTRL_PIN;
    int cs_pin = E_INK_CS_PIN;
    int rst_pin = E_INK_RST_PIN;
    int dc_pin = E_INK_DC_PIN;
    int qr_version = QRCODE_VERSION;
    int qr_block_size = 4;
    enum eink_orientation rotate = ROTATE_90;
    //
    int init_hw();
    int set_xy_counter(uint16_t x, uint16_t y);
    int set_xy_window(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end);
    int eink_init_controller(void);
    int eink_hw_rst(void);
    int eink_send_command(uint8_t cmd);
    int eink_send_data(uint8_t data);
    int eink_send_data(uint8_t *data, uint16_t data_len);
    int eink_wait_for_idle(void);
    void set_abs_pixel_to_buf(int x, int y, uint8_t *buf, bool invert);
    void set_pixel_to_buf(int x, int y, uint8_t *buf, bool invert);
    void print_char_to_buf(int x, int y, uint8_t *buf, char ascii_char, struct font_data *font, bool invert);
    void print_line_to_buf(int x, int y, uint8_t *buf, const char *p_text, struct font_data *font, bool invert);
    int set_pixel_in_canvas(uint8_t pixel, uint16_t pos_x, uint16_t pos_y, uint8_t *canvas_buf, uint16_t canvas_width, uint16_t canvas_height);

public:
    int turn_on();
    int turn_off();
    int soft_sleep();
    int clear(enum eink_color_id color);
    int draw_img(uint8_t *buf, uint16_t pos_x = 0, uint16_t pos_y = 0, uint16_t width = EPD_WIDTH, uint16_t height = EPD_HEIGHT);
    int print_line(uint16_t line_num, uint16_t pos_y, char *line);
    int update(void);
    //
    int print_qr(int pos_x, int pos_y, const char *data);
};

extern BEINKClass BEINK;

#endif