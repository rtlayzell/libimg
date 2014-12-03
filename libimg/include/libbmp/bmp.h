#ifndef __bmp_h__
#define __bmp_h__

#ifdef __cplusplus
#define BMP_BEGIN_DECLS extern "C" {
#define BMP_END_DECLS }
#else
#define BMP_BEGIN_DECLS
#define BMP_END_DECLS
#endif

BMP_BEGIN_DECLS

#ifndef bool
typedef int bool_t;
#define FALSE (0)
#define TRUE !FALSE
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif


typedef enum {
	BI_RGB = 0,
	BI_RLE8,
	BI_RLE4,
	BI_BITFIELDS,
	BI_JPEG,
	BI_PNG,
} bmp_compression_method_t;

typedef struct {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;
} rgb_pixel_t;

typedef struct {
	uint8_t magic[2];   /* the magic number used to identify the BMP file:
						0x42 0x4D (Hex code points for B and M).
						The following entries are possible:
						BM - Windows 3.1x, 95, NT, ... etc
						BA - OS/2 Bitmap Array
						CI - OS/2 Color Icon
						CP - OS/2 Color Pointer
						IC - OS/2 Icon
						PT - OS/2 Pointer. */
	uint32_t filesz;    /* the size of the BMP file in bytes */
	uint16_t creator1;  /* reserved. */
	uint16_t creator2;  /* reserved. */
	uint32_t offset;    /* the offset, i.e. starting address,
						of the byte where the bitmap data can be found. */
} bmp_header_t;

typedef struct {
	uint32_t header_sz;     /* the size of this header (40 bytes) */
	uint32_t width;         /* the bitmap width in pixels */
	uint32_t height;        /* the bitmap height in pixels */
	uint16_t nplanes;       /* the number of color planes being used.
							Must be set to 1. */
	uint16_t depth;         /* the number of bits per pixel,
							which is the color depth of the image.
							Typical values are 1, 4, 8, 16, 24 and 32. */
	uint32_t compress_type; /* the compression method being used.
							See also bmp_compression_method_t. */
	uint32_t bmp_bytesz;    /* the image size. This is the size of the raw bitmap
							data (see below), and should not be confused
							with the file size. */
	uint32_t hres;          /* the horizontal resolution of the image.
							(pixel per meter) */
	uint32_t vres;          /* the vertical resolution of the image.
							(pixel per meter) */
	uint32_t ncolors;       /* the number of colors in the color palette,
							or 0 to default to 2<sup><i>n</i></sup>. */
	uint32_t nimpcolors;    /* the number of important colors used,
							or 0 when every color is important;
							generally ignored. */
} bmp_dib_v3_header_t;

typedef struct _bmp_struct bmp_struct;
typedef bmp_struct const* bmp_const_structp;
typedef bmp_struct* bmp_structp;
typedef bmp_struct** bmp_structpp;

typedef void(*bmp_rw_ptr)(bmp_structp bmp, uint8_t* data, uint32_t length);

void bmp_destroy_struct(bmp_structp bmp);
void bmp_destroy_read_struct(bmp_structp bmp);
void bmp_destroy_write_struct(bmp_structp bmp);


bmp_structp bmp_create_bitmap(uint32_t width, uint32_t height, uint16_t depth);

bmp_structp bmp_create_struct();
bmp_structp bmp_create_read_struct();
bmp_structp bmp_create_write_struct();

bool_t bmp_set_read_fn(bmp_structp bmp, void* io_ptr, bmp_rw_ptr fn);
bool_t bmp_set_write_fn(bmp_structp bmp, void* io_ptr, bmp_rw_ptr fn);

void* bmp_get_io_ptr(bmp_const_structp bmp);

uint32_t bmp_get_width(bmp_const_structp bmp);
uint32_t bmp_get_height(bmp_const_structp  bmp);

uint32_t bmp_get_pixel_offset(bmp_const_structp bmp);
uint16_t bmp_get_creator1(bmp_const_structp bmp);
uint16_t bmp_get_creator2(bmp_const_structp bmp);

uint32_t bmp_get_filesz(bmp_const_structp bmp);
uint16_t bmp_get_depth(bmp_const_structp bmp);
uint32_t bmp_get_dpi(bmp_const_structp bmp);
uint32_t bmp_get_dpi_x(bmp_const_structp bmp);
uint32_t bmp_get_dpi_y(bmp_const_structp bmp);

void bmp_read_header(bmp_structp bmp);
void bmp_read_dib(bmp_structp bmp);
void bmp_read_palette(bmp_structp bmp);
void bmp_read_pixels(bmp_structp bmp, uint32_t offset, uint32_t length);

void bmp_write_header(bmp_structp bmp);
void bmp_write_dib(bmp_structp bmp);
void bmp_write_palette(bmp_structp bmp);
void bmp_write_pixels(bmp_structp bmp, uint32_t offset, uint32_t length);

BMP_END_DECLS

#endif /*__bmp_h__*/