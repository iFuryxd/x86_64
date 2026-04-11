#include <kernel/vga.h>
#include <common/print.h>
#include <common/string.h>


#define U8(str) ((uint8_t*)(str))

typedef struct {
uint8_t* writeas;
uint8_t* msg;
enum color color;
}__writeas;

static void set_col(enum color color) {
switch(color) {
        case black: vga_set_color(vga_make_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE)); break;
        case green: vga_set_color(vga_make_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK)); break;
        case cyan: vga_set_color(vga_make_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK)); break;
        case red: vga_set_color(vga_make_color(VGA_COLOR_RED, VGA_COLOR_BLACK)); break;
        case magenta: vga_set_color(vga_make_color(VGA_COLOR_MAGENTA, VGA_COLOR_BLACK)); break;
        case brown: vga_set_color(vga_make_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK)); break;
        case l_gray: vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK)); break;
        case d_gray: vga_set_color(vga_make_color(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK)); break;
        case l_blue: vga_set_color(vga_make_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK)); break;
        case l_green: vga_set_color(vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK)); break;
        case l_cyan: vga_set_color(vga_make_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK)); break;
        case l_red: vga_set_color(vga_make_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK)); break;
        case l_magenta: vga_set_color(vga_make_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK)); break;
        case l_brown: vga_set_color(vga_make_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK)); break;
        case white: vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)); break;
        default: vga_set_color(vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    };
}

void print(uint8_t* caller, const char* msg, enum color color) {
    __writeas as = {
        caller,
        U8(msg),
        color,
    };
    set_col(as.color);
    uint32_t size = strlen(as.writeas) + strlen(as.msg)+1;
    uint8_t buff[size];
    strconcat(buff, as.writeas, as.msg);
    vga_write(buff);
}

