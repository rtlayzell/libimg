#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\..\include\libbmp\bmp.h"
#include "..\..\include\libbmp\bmpdef.h"

void bmp_destroy_struct(bmp_structp bmp)
{
	bmp_free_pixels(bmp);
	bmp_free_colors(bmp);
	free(bmp);
}

bmp_structp bmp_create_struct()
{
	bmp_structp result = NULL;
	result = malloc(sizeof(bmp_struct));

	memset((void*)&result->header, 0, sizeof(bmp_header_t));
	memset((void*)&result->dib, 0, sizeof(bmp_dib_v3_header_t));

	result->pixels = NULL;
	result->colors = NULL;
	result->read_fn = NULL;
	result->write_fn = NULL;
	result->io_ptr = NULL;

	return result;
}


void* bmp_get_io_ptr(bmp_const_structp bmp)
{
	return bmp->io_ptr;
}

uint32_t bmp_get_width(bmp_const_structp bmp)
{
	return bmp->dib.width;
}

uint32_t bmp_get_height(bmp_const_structp bmp)
{
	return bmp->dib.height;
}

uint32_t bmp_get_pixel_offset(bmp_const_structp bmp)
{
	return bmp->header.offset;
}

uint16_t bmp_get_creator1(bmp_const_structp bmp)
{
	return bmp->header.creator1;
}

uint16_t bmp_get_creator2(bmp_const_structp bmp)
{
	return bmp->header.creator2;
}


uint32_t bmp_get_filesz(bmp_const_structp bmp)
{
	return bmp->header.filesz;
}

uint16_t bmp_get_depth(bmp_const_structp bmp)
{
	return bmp->dib.depth;
}

uint32_t bmp_get_dpi(bmp_const_structp bmp)
{
	// ?? are the hres and vres always the same? probably not..
	return bmp_get_dpi_x(bmp);
}

uint32_t bmp_get_dpi_x(bmp_const_structp bmp)
{
	return (uint32_t)round(bmp->dib.hres / DPI_FACTOR);
}

uint32_t bmp_get_dpi_y(bmp_const_structp bmp)
{
	return (uint32_t)round(bmp->dib.vres / DPI_FACTOR);
}


