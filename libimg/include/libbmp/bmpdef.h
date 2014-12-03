#ifndef __bmp_def_h__
#define __bmp_def_h__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

#define DEFAULT_DPI_X 3780
#define DEFAULT_DPI_Y 3780
#define DPI_FACTOR 39.37007874015748

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

	rgb_pixel_t *pixels;
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

void bmp_malloc_pixels(bmp_structp bmp)
{
	uint32_t i;
	uint32_t length;

	length = bmp->dib.width * bmp->dib.height;
	bmp->pixels = (rgb_pixel_t*)malloc(sizeof(rgb_pixel_t) * length);

	for (i = 0; i < length; ++i)
		memset(&bmp->pixels[i], 0xff, sizeof(rgb_pixel_t));
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
	if (bmp->pixels == NULL)
		return;

	if (bmp->pixels)
	{
		free(bmp->pixels);
		bmp->pixels = NULL;
	}
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
	{
		free(bmp->colors);
		bmp->colors = NULL;
	}
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

#define INT_SQUARE(v) ((int)((v) * (v)))

static int
find_closest_color(bmp_structp bmp, rgb_pixel_t pixel)
{
	uint32_t i, best = 0;
	int best_match = 999999;

	for (i = 0; i < bmp->dib.ncolors; ++i) {
		rgb_pixel_t color = bmp->colors[i];
		int temp_match = INT_SQUARE(color.red - pixel.red) +
			INT_SQUARE(color.green - pixel.green) +
			INT_SQUARE(color.blue - pixel.blue);

		if (temp_match < best_match) {
			best = i;
			best_match = temp_match;
		}

		if (best_match < 1)
			break;
	}

	return best;
}

static void
bmp_get_row_data_for_1(bmp_structp bmp, unsigned char *buf, size_t buf_len,
uint32_t row)
{
	uint8_t pos_weights[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	uint32_t i = 0, j, k = 0;
	uint32_t index;

	if (bmp->dib.width > 8 * buf_len) return;

	while (i < bmp->dib.width) {
		for (j = 0, index = 0; j < 8 && i < bmp->dib.width; ++i, ++j)
			index += pos_weights[j] * find_closest_color(bmp, bmp->pixels[i + bmp->dib.width * row]);

		buf[k++] = index & 0xff;
	}
}

static void
bmp_get_row_data_for_4(bmp_structp bmp, unsigned char *buf, size_t buf_len,
uint32_t row)
{
	uint8_t pos_weights[2] = { 16, 1 };
	uint32_t i = 0, j, k = 0;
	uint32_t index;

	if (bmp->dib.width > 2 * buf_len) return;

	while (i < bmp->dib.width) {
		for (j = 0, index = 0; j < 2 && i < bmp->dib.width; ++i, ++j)
			index += pos_weights[j] * find_closest_color(bmp, bmp->pixels[i + bmp->dib.width * row]);

		buf[k++] = index & 0xff;
	}
}

static void
bmp_get_row_data_for_8(bmp_structp bmp, unsigned char *buf, size_t buf_len, uint32_t row)
{
	uint32_t i;

	if (bmp->dib.width > buf_len) return;

	for (i = 0; i < bmp->dib.width; ++i)
		buf[i] = find_closest_color(bmp, bmp->pixels[i + bmp->dib.width * row]);
}

static void
bmp_get_row_data_for_24(bmp_structp bmp, unsigned char *buf, size_t buf_len, uint32_t row)
{
	uint32_t i;

	if (bmp->dib.width * 3 > buf_len) return;

	for (i = 0; i < bmp->dib.width; ++i)
		memcpy(buf + 3 * i, (uint8_t *)&(bmp->pixels[i + bmp->dib.width * row]), 3);
}

static void
bmp_get_row_data_for_32(bmp_structp bmp, unsigned char *buf, size_t buf_len, uint32_t row)
{
	uint32_t i;

	if (bmp->dib.width * 4 > buf_len) return;

	for (i = 0; i < bmp->dib.width; ++i)
		memcpy(buf + 4 * i, (uint8_t *)&(bmp->pixels[i + bmp->dib.width * row]), 4);
}


#endif __bmp_def_h__
