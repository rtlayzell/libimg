#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\..\include\libbmp\bmp.h"

static void bmp_malloc_colors(bmp_structp bmp);
static void bmp_malloc_pixels(bmp_structp bmp);
static void bmp_free_colors(bmp_structp bmp);
static void bmp_free_pixels(bmp_structp bmp);

static uint32_t uint32_pow(uint32_t base, uint32_t depth);
static void bmp_create_standard_color_table(bmp_structp bmp);

struct _bmp_struct
{
	bmp_header_t header;
	bmp_dib_v3_header_t dib;

	rgb_pixel_t **pixels;
	rgb_pixel_t *colors;

	bmp_rw_ptr read_fn;
	bmp_rw_ptr write_fn;
	void* io_ptr;
};


static bool_t
_is_big_endian(void)
{
	uint16_t value = 0x0001;
	return (*(uint8_t *)&value) != 0x01;
}

#define UINT16_SWAP_LE_BE_CONSTANT(val)		\
  ((uint16_t)					\
   (						\
    (uint16_t) ((uint16_t) (val) >> 8) |	\
    (uint16_t) ((uint16_t) (val) << 8)))

#define UINT32_SWAP_LE_BE_CONSTANT(val)			  \
  ((uint32_t)						  \
   (							  \
    (((uint32_t) (val) & (uint32_t) 0x000000ffU) << 24) | \
    (((uint32_t) (val) & (uint32_t) 0x0000ff00U) <<  8) | \
    (((uint32_t) (val) & (uint32_t) 0x00ff0000U) >>  8) | \
    (((uint32_t) (val) & (uint32_t) 0xff000000U) >> 24)))

static void
bmp_header_swap_endianess(bmp_header_t *header)
{
	header->filesz = UINT32_SWAP_LE_BE_CONSTANT(header->filesz);
	header->creator1 = UINT16_SWAP_LE_BE_CONSTANT(header->creator1);
	header->creator2 = UINT16_SWAP_LE_BE_CONSTANT(header->creator2);
	header->offset = UINT32_SWAP_LE_BE_CONSTANT(header->offset);
}

static void
bmp_dib_v3_header_swap_endianess(bmp_dib_v3_header_t *dib)
{
	dib->header_sz = UINT32_SWAP_LE_BE_CONSTANT(dib->header_sz);
	dib->width = UINT32_SWAP_LE_BE_CONSTANT(dib->width);
	dib->height = UINT32_SWAP_LE_BE_CONSTANT(dib->height);
	dib->nplanes = UINT16_SWAP_LE_BE_CONSTANT(dib->nplanes);
	dib->depth = UINT16_SWAP_LE_BE_CONSTANT(dib->depth);
	dib->compress_type = UINT32_SWAP_LE_BE_CONSTANT(dib->compress_type);
	dib->bmp_bytesz = UINT32_SWAP_LE_BE_CONSTANT(dib->bmp_bytesz);
	dib->hres = UINT32_SWAP_LE_BE_CONSTANT(dib->hres);
	dib->vres = UINT32_SWAP_LE_BE_CONSTANT(dib->vres);
	dib->ncolors = UINT32_SWAP_LE_BE_CONSTANT(dib->ncolors);
	dib->nimpcolors = UINT32_SWAP_LE_BE_CONSTANT(dib->nimpcolors);
}


void bmp_destroy_read_struct(bmp_structp bmp)
{
	bmp_free_pixels(bmp);
	bmp_free_colors(bmp);
	free(bmp);
}

void bmp_destroy_write_struct(bmp_structp bmp)
{

}

bmp_structp bmp_create_struct()
{
	bmp_structp result = NULL;
	result = malloc(sizeof(bmp_struct));

	result->pixels = NULL;
	result->colors = NULL;
	result->read_fn = NULL;
	result->write_fn = NULL;
	result->io_ptr = NULL;

	return result;
}

bmp_structp bmp_create_read_struct()
{
	return bmp_create_struct();
}

bmp_structp bmp_create_write_struct()
{
	return bmp_create_struct();
}

bool_t bmp_set_read_fn(bmp_structp bmp, void* io_ptr, bmp_rw_ptr fn)
{
	bmp->read_fn = fn;
	bmp->io_ptr = io_ptr;

	return TRUE;
}

bool_t bmp_set_write_fn(bmp_structp bmp, void* io_ptr, bmp_rw_ptr fn)
{
	bmp->write_fn = fn;
	bmp->io_ptr = io_ptr;

	return TRUE;
}

void* bmp_get_io_ptr(bmp_structp bmp)
{
	return bmp->io_ptr;
}

uint32_t bmp_get_width(bmp_structp bmp)
{
	return bmp->dib.width;
}

uint32_t bmp_get_height(bmp_structp bmp)
{
	return bmp->dib.height;
}

uint32_t bmp_get_dpi(bmp_structp bmp)
{
	return 0;
}

uint32_t bmp_get_dpi_x(bmp_structp bmp)
{
	return 0;
}

uint32_t bmp_get_dpi_y(bmp_structp bmp)
{
	return 0;
}

void
bmp_read_header_info(bmp_structp bmp)
{
	bmp->read_fn(bmp, (uint8_t*)&bmp->header.magic, sizeof(uint8_t) * 2);
	bmp->read_fn(bmp, (uint8_t*)&bmp->header.filesz, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->header.creator1, sizeof(uint16_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->header.creator2, sizeof(uint16_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->header.offset, sizeof(uint32_t));

	if (_is_big_endian()) 
		bmp_header_swap_endianess(&bmp->header);
}

void
bmp_read_dib(bmp_structp bmp)
{
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.header_sz, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.width, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.height, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.nplanes, sizeof(uint16_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.depth, sizeof(uint16_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.compress_type, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.bmp_bytesz, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.hres, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.vres, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.ncolors, sizeof(uint32_t));
	bmp->read_fn(bmp, (uint8_t*)&bmp->dib.nimpcolors, sizeof(uint32_t));

	if (_is_big_endian()) 
		bmp_dib_v3_header_swap_endianess(&bmp->dib);
}

void 
bmp_read_palette(bmp_structp bmp)
{
	if (bmp->dib.depth == 1 || bmp->dib.depth == 4 || bmp->dib.depth == 8)
	{
		if (bmp->colors != NULL) 
			bmp_free_colors(bmp);
		
		bmp_malloc_colors(bmp);
		bmp->read_fn(bmp, (uint8_t*)bmp->colors, 
			sizeof(rgb_pixel_t) * bmp->dib.ncolors);
	}
	else if (bmp->dib.depth == 16) 
	{ /* the bit masks, not palette */
		uint16_t red_mask = 63488;  /* bits 1-5 */
		uint16_t green_mask = 2016; /* bits 6-11 */
		uint16_t blue_mask = 31;    /* bits 12-16 */
		uint16_t zero_word = 0;

		if (_is_big_endian()) {
			red_mask = UINT16_SWAP_LE_BE_CONSTANT(red_mask);
			green_mask = UINT16_SWAP_LE_BE_CONSTANT(green_mask);
			blue_mask = UINT16_SWAP_LE_BE_CONSTANT(blue_mask);
		}

		bmp->read_fn(bmp, (uint8_t*)&red_mask, sizeof(uint16_t));
		bmp->read_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));

		bmp->read_fn(bmp, (uint8_t*)&green_mask, sizeof(uint16_t));
		bmp->read_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));

		bmp->read_fn(bmp, (uint8_t*)&blue_mask, sizeof(uint16_t));
		bmp->read_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));
	}
}

void bmp_malloc_colors(bmp_structp bmp)
{
	bmp->dib.ncolors = uint32_pow(2, bmp->dib.depth);
	if (bmp->dib.depth == 1 || bmp->dib.depth == 4 || bmp->dib.depth == 8) {
		bmp->colors = (rgb_pixel_t*)malloc(sizeof(rgb_pixel_t) * bmp->dib.ncolors);
		bmp_create_standard_color_table(bmp);
	}
}


/**
* Free the memory of pixels
*/
void
bmp_free_pixels(bmp_structp bmp)
{
	uint32_t i;
	if (bmp->pixels == NULL)
		return;

	for (i = 0; i < bmp->dib.width; ++i)
		free(bmp->pixels[i]);

	free(bmp->pixels), bmp->pixels = NULL;
}

/**
* Free the memory of color palette
*/
void
bmp_free_colors(bmp_structp bmp)
{
	if (bmp->colors == NULL)
		return;

	if (bmp->colors)
		free(bmp->colors);
}


/**
* Create the standard color table for BMP object
*/
void
bmp_create_standard_color_table(bmp_structp bmp)
{
	int i, j, k, ell;

	switch (bmp->dib.depth) {
	case 1:
		for (i = 0; i < 2; ++i) {
			bmp->colors[i].red = i * 255;
			bmp->colors[i].green = i * 255;
			bmp->colors[i].blue = i * 255;
			bmp->colors[i].alpha = 0;
		}
		break;

	case 4:
		i = 0;
		for (ell = 0; ell < 2; ++ell) {
			for (k = 0; k < 2; ++k) {
				for (j = 0; j < 2; ++j) {
					bmp->colors[i].red = j * 128;
					bmp->colors[i].green = k * 128;
					bmp->colors[i].blue = ell * 128;
					bmp->colors[i].alpha = 0;
					++i;
				}
			}
		}

		for (ell = 0; ell < 2; ++ell) {
			for (k = 0; k < 2; ++k) {
				for (j = 0; j < 2; ++j) {
					bmp->colors[i].red = j * 255;
					bmp->colors[i].green = k * 255;
					bmp->colors[i].blue = ell * 255;
					bmp->colors[i].alpha = 0;
					++i;
				}
			}
		}

		i = 8;
		bmp->colors[i].red = 192;
		bmp->colors[i].green = 192;
		bmp->colors[i].blue = 192;
		bmp->colors[i].alpha = 0;

		break;

	case 8:
		i = 0;
		for (ell = 0; ell < 4; ++ell) {
			for (k = 0; k < 8; ++k) {
				for (j = 0; j < 8; ++j) {
					bmp->colors[i].red = j * 32;
					bmp->colors[i].green = k * 32;
					bmp->colors[i].blue = ell * 64;
					bmp->colors[i].alpha = 0;
					++i;
				}
			}
		}

		i = 0;
		for (ell = 0; ell < 2; ++ell) {
			for (k = 0; k < 2; ++k) {
				for (j = 0; j < 2; ++j) {
					bmp->colors[i].red = j * 128;
					bmp->colors[i].green = k * 128;
					bmp->colors[i].blue = ell * 128;
					++i;
				}
			}
		}

		// overwrite colors 7, 8, 9
		i = 7;
		bmp->colors[i].red = 192;
		bmp->colors[i].green = 192;
		bmp->colors[i].blue = 192;
		i++; // 8
		bmp->colors[i].red = 192;
		bmp->colors[i].green = 220;
		bmp->colors[i].blue = 192;
		i++; // 9
		bmp->colors[i].red = 166;
		bmp->colors[i].green = 202;
		bmp->colors[i].blue = 240;

		// overwrite colors 246 to 255 
		i = 246;
		bmp->colors[i].red = 255;
		bmp->colors[i].green = 251;
		bmp->colors[i].blue = 240;
		i++; // 247
		bmp->colors[i].red = 160;
		bmp->colors[i].green = 160;
		bmp->colors[i].blue = 164;
		i++; // 248
		bmp->colors[i].red = 128;
		bmp->colors[i].green = 128;
		bmp->colors[i].blue = 128;
		i++; // 249
		bmp->colors[i].red = 255;
		bmp->colors[i].green = 0;
		bmp->colors[i].blue = 0;
		i++; // 250
		bmp->colors[i].red = 0;
		bmp->colors[i].green = 255;
		bmp->colors[i].blue = 0;
		i++; // 251
		bmp->colors[i].red = 255;
		bmp->colors[i].green = 255;
		bmp->colors[i].blue = 0;
		i++; // 252
		bmp->colors[i].red = 0;
		bmp->colors[i].green = 0;
		bmp->colors[i].blue = 255;
		i++; // 253
		bmp->colors[i].red = 255;
		bmp->colors[i].green = 0;
		bmp->colors[i].blue = 255;
		i++; // 254
		bmp->colors[i].red = 0;
		bmp->colors[i].green = 255;
		bmp->colors[i].blue = 255;
		i++; // 255
		bmp->colors[i].red = 255;
		bmp->colors[i].green = 255;
		bmp->colors[i].blue = 255;
		break;
	}
}

uint32_t
uint32_pow(uint32_t base, uint32_t depth)
{
	uint32_t i, result = 1;

	for (i = 0; i < depth; ++i)
		result *= base;

	return result;
}