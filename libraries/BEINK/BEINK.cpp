///
/// @file BEINK.cpp
/// @author Anuj Pathak (anuj@biii.in)
/// @brief
/// @version 0.1
/// @date 2021-02-09
///
/// @copyright Copyright (c) 2021
///
///
#include "BEINK.h"
#include <qrcode.h>

BEINKClass BEINK;

#define EINK_CMD(cmd_byte) \
    {                      \
        .cmd = cmd_byte,   \
        .data = 0,         \
        .data_len = 0,     \
    }

#define EINK_CMD_AND_DATA(cmd_byte, ...)              \
    {                                                 \
        .cmd = cmd_byte,                              \
        .data = (uint8_t[]){__VA_ARGS__},             \
        .data_len = sizeof((uint8_t[]){__VA_ARGS__}), \
    }

struct eink_cmd {
    uint8_t cmd;
    uint8_t *data;
    uint8_t data_len;
};
static const struct eink_cmd init_ctrl_cmds[] = {
    EINK_CMD_AND_DATA(DRIVER_OUTPUT, 0xD3, 0x00, 0x00),
    EINK_CMD_AND_DATA(DATA_ENTRY_SEQUENCE, 0x03), // set xy increment
    EINK_CMD_AND_DATA(RAM_X_START_END, 0x00, 0x0C),
    EINK_CMD_AND_DATA(RAM_Y_START_END, 0x00, 0x00, 0xD3, 0x00),
    EINK_CMD_AND_DATA(BORDER_WAVEFORM, 0x05),
    EINK_CMD_AND_DATA(DISPLAY_UPDATE_CTRL, 0x00, 0x80),
    EINK_CMD_AND_DATA(0x18, 0x80),
    EINK_CMD_AND_DATA(DISPLAY_UPDATE_SEQUENCE_CFG, 0xB1),
    EINK_CMD(DISPLAY_UPDATE_SEQUENCE_RUN),
};

int BEINKClass::eink_wait_for_idle(void)
{
    while (digitalRead(busy_pin) == HIGH) {
    }
    return 0;
}

int BEINKClass::eink_send_data(uint8_t *data, uint16_t data_len)
{
    digitalWrite(cs_pin, LOW);
    SPI.transfer(data, data_len);
    digitalWrite(cs_pin, HIGH);
    return 0;
}

int BEINKClass::eink_send_data(uint8_t data)
{
    eink_send_data(&data, sizeof(data));
    return 0;
}

int BEINKClass::eink_send_command(uint8_t cmd)
{
    digitalWrite(dc_pin, LOW);
    eink_send_data(cmd);
    digitalWrite(dc_pin, HIGH);
    return 0;
}

int BEINKClass::eink_hw_rst(void)
{
    digitalWrite(rst_pin, LOW);
    delay(1);
    digitalWrite(rst_pin, HIGH);
    delay(1);
    return 0;
}

int BEINKClass::eink_init_controller(void)
{
    eink_send_command(SW_RESET);
    eink_wait_for_idle();
    int cmds = (sizeof(init_ctrl_cmds) / sizeof(init_ctrl_cmds[0]));
    for (int i = 0; i < cmds; i++) {
        eink_send_command(init_ctrl_cmds[i].cmd);
        if (init_ctrl_cmds[i].data) {
            eink_send_data(init_ctrl_cmds[i].data, init_ctrl_cmds[i].data_len);
        }
    }
    eink_wait_for_idle();
    return 0;
}

int BEINKClass::init_hw(void)
{
    pinMode(busy_pin, INPUT);
    pinMode(vcc_pin, OUTPUT_HIGH_DRIVE);
    pinMode(cs_pin, OUTPUT);
    pinMode(rst_pin, OUTPUT);
    pinMode(dc_pin, OUTPUT);
    //
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.begin();
    //
    digitalWrite(cs_pin, HIGH);
    digitalWrite(dc_pin, HIGH);
    digitalWrite(rst_pin, HIGH);
    digitalWrite(vcc_pin, HIGH);
    //
    return 0;
}

int BEINKClass::set_xy_window(uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end)
{
    uint8_t x_data[] = {
        (x_start & 0xFF),
        (x_end & 0xFF),
    };
    uint8_t y_data[] = {
        ((y_start >> 0) & 0xFF),
        ((y_start >> 8) & 0xFF),
        ((y_end >> 0) & 0xFF),
        ((y_end >> 8) & 0xFF),
    };
    eink_send_command(RAM_X_START_END);
    eink_send_data(x_data, sizeof(x_data));
    eink_send_command(RAM_Y_START_END);
    eink_send_data(y_data, sizeof(y_data));
    return 0;
}

int BEINKClass::set_xy_counter(uint16_t x, uint16_t y)
{
    uint8_t x_data[] = {
        x,
    };
    uint8_t y_data[] = {
        ((y >> 0) & 0xFF),
        ((y >> 8) & 0xFF),
    };
    eink_send_command(RAM_X_COUNTER);
    eink_send_data(x_data, sizeof(x_data));
    eink_send_command(RAM_Y_COUNTER);
    eink_send_data(y_data, sizeof(y_data));
    return 0;
}

int BEINKClass::update(void)
{
    eink_send_command(DISPLAY_UPDATE_SEQUENCE_CFG);
    eink_send_data(0xC7);
    eink_send_command(DISPLAY_UPDATE_SEQUENCE_RUN);
    eink_wait_for_idle();
    return 0;
}

int BEINKClass::turn_on(void)
{
    init_hw();
    eink_hw_rst();
    eink_init_controller();
    return 0;
}

int BEINKClass::turn_off(void)
{
    return 0;
}

int BEINKClass::soft_sleep(void)
{
    eink_send_command(DEEP_SLEEP);
    eink_send_data(0x01);
    return 0;
}

int BEINKClass::clear(enum eink_color_id color)
{
    int bts = (width >> 3) * height;
    for (int i = 0; i < 2; i++) {
        set_xy_window(0, (width >> 3) - 1, 0, height - 1);
        set_xy_counter(0, 0);
        if (i == 0) {
            eink_send_command(BW_RAM_ADDRESS);
        } else {
            eink_send_command(RED_RAM_ADDRESS);
        }
        for (int b = 0; b < bts; b++) {
            eink_send_data(color == i ? 0xFF : 0x00);
        }
    }
    // eink_update();
    return 0;
}

int BEINKClass::draw_img(uint8_t *buf, uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height)
{
    int bts = (width >> 3) * height;
    for (int i = 0; i < 2; i++) {
        set_xy_window(pos_x, (width >> 3) - 1, pos_y, height - 1);
        set_xy_counter(pos_x, pos_y);
        if (i == 0) {
            eink_send_command(BW_RAM_ADDRESS);
        } else {
            eink_send_command(RED_RAM_ADDRESS);
        }
        for (int b = 0; b < bts; b++) {
            eink_send_data(buf[bts * i + b]);
        }
    }
    // eink_update();
    return 0;
}

int BEINKClass::set_pixel_in_canvas(uint8_t pixel, uint16_t pos_x, uint16_t pos_y, uint8_t *canvas_buf, uint16_t canvas_width, uint16_t canvas_height)
{
    switch (this->rotate) {
    case ROTATE_0: {
    } break;
    case ROTATE_180: {
        pos_x = canvas_width - pos_x;
        pos_y = canvas_height - pos_y;
    } break;
    case ROTATE_90: {
        uint16_t temp = pos_x;
        pos_x = canvas_width - pos_y;
        pos_y = temp;
    } break;
    case ROTATE_270: {
        uint16_t temp = pos_x;
        pos_x = pos_y;
        pos_y = canvas_height - temp;
    } break;
    }
    // Serial.print(pos_x, HEX);
    // Serial.print(":");
    // Serial.print(pos_y, HEX);
    // Serial.print("  ");
    if (pos_x >= canvas_width || pos_y >= canvas_height) {
        return -1;
    }
    //
    if (pixel) {
        canvas_buf[(canvas_width * pos_y + pos_x) >> 3] |= (0x80 >> (pos_x & 0x07));
    } else {
        canvas_buf[(canvas_width * pos_y + pos_x) >> 3] &= ~(0x80 >> (pos_x & 0x07));
    }
}

int BEINKClass::print_line(uint16_t pos_x, uint16_t pos_y, char *line)
{
    struct font_data *font = &Font16;
    uint16_t char_cnt = strlen(line);
    uint16_t canvas_width;
    uint16_t canvas_height;
    //
    switch (this->rotate) {
    case ROTATE_0:
    case ROTATE_180:
        canvas_width = char_cnt * font->Width;
        canvas_height = font->Height;
        break;
    case ROTATE_90:
    case ROTATE_270:
        canvas_width = font->Height;
        canvas_height = char_cnt * font->Width;
        break;
    }
    canvas_width = (canvas_width + 7) & 0xFFF8;
    canvas_height = (canvas_height + 7) & 0xFFF8;
    uint16_t canvas_size = (canvas_width * canvas_height) >> 3;
    uint8_t canvas_buf[canvas_size];
    memset(canvas_buf, 0xFF, canvas_size);
    //
    uint16_t off_x, off_y;
    const uint8_t *char_pixels;
    for (int char_idx = 0; char_idx < char_cnt; char_idx++) {
        char_pixels = &font->table[(line[char_idx] - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0))];
        for (int char_off_y = 0; char_off_y < font->Height; char_off_y++) {
            for (int char_off_x = 0; char_off_x < font->Width; char_off_x++) {
                uint8_t bit = char_pixels[char_off_x >> 3] & (0x80 >> (char_off_x & 0x07));
                uint8_t bit_pos_x = (char_idx * font->Width) + char_off_x;
                set_pixel_in_canvas(bit == 0, bit_pos_x, char_off_y, canvas_buf, canvas_width, canvas_height);
            }
            char_pixels += ((font->Width + 7) >> 3);
            Serial.println();
        }
        Serial.println();
    }
    //
    set_xy_window(pos_x >> 3, ((canvas_width + pos_x) >> 3) - 1, pos_y, pos_y + canvas_height - 1);
    set_xy_counter(pos_x >> 3, pos_y);
    eink_send_command(BW_RAM_ADDRESS);
    for (int b = 0; b < canvas_size; b++) {
        eink_send_data(canvas_buf[b]);
    }
    // eink_update();
    return 0;
}

int BEINKClass::print_qr(int pos_x, int pos_y, const char *data)
{
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(qr_version)];
    qrcode_initText(&qrcode, qrcodeData, qr_version, ECC_MEDIUM, data);
    uint16_t canvas_width = (qr_block_size * qrcode.size + 7) & 0xFFF8;
    uint16_t canvas_height = (qr_block_size * qrcode.size + 7) & 0xFFF8;
    uint16_t canvas_size = (canvas_width * canvas_height) >> 3;
    uint8_t canvas_buf[canvas_size];
    memset(canvas_buf, 0xFF, canvas_size);
    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            int qr_bit = qrcode_getModule(&qrcode, x, y);
            // Serial.print(qr_bit ? '#' : ' ');
            for (int qbl = 0; qbl < qr_block_size; qbl++) {
                for (int qbh = 0; qbh < qr_block_size; qbh++) {
                    set_pixel_in_canvas(qr_bit == 0, qr_block_size * x + qbl, qr_block_size * y + qbh, canvas_buf, canvas_width, canvas_height);
                }
            }
        }
        // Serial.println();
    }
    set_xy_window(pos_x >> 3, ((canvas_width + pos_x) >> 3) - 1, pos_y, pos_y + canvas_height - 1);
    set_xy_counter(pos_x >> 3, pos_y);
    eink_send_command(BW_RAM_ADDRESS);
    for (int b = 0; b < canvas_size; b++) {
        eink_send_data(canvas_buf[b]);
    }
    // eink_update();
    return 0;
}
