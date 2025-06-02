#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

static int fd_fb;
static struct fb_var_screeninfo var;
static int screen_size;
static unsigned char *fbmem;
static unsigned int line_width;
static unsigned int pixel_width;

void lcd_put_pixel(int x, int y, unsigned int color) {
	unsigned char *pen_8 = fbmem+y * line_width + x * pixel_width;
    unsigned short *pen_16;
    unsigned int *pen_32;

    unsigned int red, green, blue;

    pen_16 = (unsigned short *)pen_8;
    pen_32 = (unsigned int *)pen_8;

    switch (var.bits_per_pixel)
    {
        case 8:
        {
            *pen_8 = color;
            break;
        }
        case 16:
        {
            red = (color >> 16) & 0xff;
            green = (color >> 8) & 0xff;
            blue = (color >> 0) & 0xff;
            color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
            *pen_16 = color;
            break;
        }
        case 32:
        {
            *pen_32 = color;
            break;
        }
        default:
        {
            printf("cannot support %dbpp\n", var.bits_per_pixel);
            break;
        }
    }
}

void draw_bitmap(	FT_Bitmap*  bitmap,
             		FT_Int      x,
             		FT_Int      y)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for (j = y, q = 0; j < y_max; j++, q++)
	{
		for (i = x, p = 0; i < x_max; i++, p++)
		{
			if (i < 0 || j < 0 || i >= var.xres || j >= var.yres)
				continue;

			lcd_put_pixel(i, j, bitmap->buffer[q * bitmap->width + p]);
		}
	}
}

int main(int argc, char **argv)
{
	wchar_t *chinese_str = L"臭臭泥";

	FT_Library	  library;
	FT_Face 	  face;
	int error;
    FT_Vector     pen;
	FT_GlyphSlot  slot;
	int font_size = 24;

	if (argc < 2)
	{
		printf("Usage : %s <font_file> [font_size]\n", argv[0]);
		return -1;		
	}
	if (argc == 3)
	{
		font_size = strtoul(argv[2], NULL, 0);
	}

    fd_fb = open("/dev/fb0", O_RDWR);
    if (fd_fb < 0) 
    {
        printf("cannot open /dev/fb0\n");
        return -1;
    }
    if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
    {
        printf("cannot get var\n");
        return -1;
    }

    line_width = var.xres * var.bits_per_pixel / 8;
    pixel_width = var.bits_per_pixel / 8;
    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    fbmem = (unsigned char *)mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if (fbmem == (unsigned char *)-1)
    {
        printf("cannot mmap\n");
        return -1;
    }

    memset(fbmem, 0, screen_size);

	error = FT_Init_FreeType(&library);

	error = FT_New_Face(library, argv[1], 0, &face);

	slot = face -> glyph;

	FT_Set_Pixel_Sizes(face, font_size, 0);

	error = FT_Load_Char(face, chinese_str[2], FT_LOAD_RENDER);
	if (error)
	{
		printf("FT_Load_Char error\n");
		return -1;		
	}

    draw_bitmap( &slot->bitmap,
                 var.xres/2,
                 var.yres/2);

    return 0;
}