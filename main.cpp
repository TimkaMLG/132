#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int width;
unsigned int height;
png_bytep *row_pointers;

static void read_png_file(char *filename) {
    FILE *fp = fopen(filename, "rb");
    png_byte bit_depth;
    png_byte color_type;
    unsigned int y;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();
    png_infop info = png_create_info_struct(png);
    if (!info) abort();
    if (setjmp(png_jmpbuf(png))) abort();
    png_init_io(png, fp);
    png_read_info(png, info);
    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);
    /* Read any color_type into 8bit depth, RGBA format. */
    /* See http://www.libpng.org/pub/png/libpng-manual.txt */
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    /* PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth. */
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    /* These color_type don't have an alpha channel then fill it with 0xff. */
    if (color_type == PNG_COLOR_TYPE_RGB ||
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);
    png_read_update_info(png, info);
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }
    png_read_image(png, row_pointers);
    fclose(fp);
}


static void process_png(void) {
    for (unsigned int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (unsigned int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            
            px[0]; //r
	    px[1]; //g
	    px[2]; //b
        }
    }
}

int main()
{
 read_png_file((char*)"file.png");
 process_png();
 return 0;
}
