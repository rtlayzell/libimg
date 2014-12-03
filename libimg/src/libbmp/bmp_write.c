
#include "..\..\include\libbmp\bmp.h"
#include "..\..\include\libbmp\bmpdef.h"

static bool_t bmp_validate_bit_depth(uint16_t depth);

void bmp_destroy_write_struct(bmp_structp bmp)
{
	bmp_destroy_struct(bmp);
}

bmp_structp bmp_create_write_struct()
{
	return bmp_create_struct();
}

bool_t bmp_set_write_fn(bmp_structp bmp, void* io_ptr, bmp_rw_ptr fn)
{
	if (fn == NULL)
		return FALSE;

	bmp->write_fn = fn;
	bmp->io_ptr = io_ptr; // maybe null if not needed..

	if (bmp->read_fn != NULL)
	{
		// error: can't have both read and write in the same structure.
		bmp->read_fn = NULL;
	}

	return TRUE;
}

void bmp_write_header(bmp_structp bmp)
{
	if (_is_big_endian())
		bmp_header_swap_endianess(&bmp->header);

	bmp->write_fn(bmp, (uint8_t*)&bmp->header.magic, sizeof(uint8_t) * 2);
	bmp->write_fn(bmp, (uint8_t*)&bmp->header.filesz, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->header.creator1, sizeof(uint16_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->header.creator2, sizeof(uint16_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->header.offset, sizeof(uint32_t));
}

void bmp_write_dib(bmp_structp bmp)
{
	if (_is_big_endian())
		bmp_dib_v3_header_swap_endianess(&bmp->dib);

	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.header_sz, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.width, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.height, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.nplanes, sizeof(uint16_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.depth, sizeof(uint16_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.compress_type, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.bmp_bytesz, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.hres, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.vres, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.ncolors, sizeof(uint32_t));
	bmp->write_fn(bmp, (uint8_t*)&bmp->dib.nimpcolors, sizeof(uint32_t));
}

void bmp_write_palette(bmp_structp bmp)
{
	if (bmp->dib.depth == 1 || bmp->dib.depth == 4 || bmp->dib.depth == 8)
	{
		bmp->write_fn(bmp, (uint8_t*)bmp->colors,
			sizeof(rgb_pixel_t) * bmp->dib.ncolors);
	}
	else if (bmp->dib.depth == 16)
	{
		/* the bit masks, not palette */
		uint16_t red_mask = 63488;  /* bits 1-5 */
		uint16_t green_mask = 2016; /* bits 6-11 */
		uint16_t blue_mask = 31;    /* bits 12-16 */
		uint16_t zero_word = 0;

		if (_is_big_endian()) {
			red_mask = UINT16_SWAP_LE_BE_CONSTANT(red_mask);
			green_mask = UINT16_SWAP_LE_BE_CONSTANT(green_mask);
			blue_mask = UINT16_SWAP_LE_BE_CONSTANT(blue_mask);
		}

		bmp->write_fn(bmp, (uint8_t*)&red_mask, sizeof(uint16_t));
		bmp->write_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));

		bmp->write_fn(bmp, (uint8_t*)&green_mask, sizeof(uint16_t));
		bmp->write_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));

		bmp->write_fn(bmp, (uint8_t*)&blue_mask, sizeof(uint16_t));
		bmp->write_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));
	}
}

void
bmp_write_pixels(bmp_structp bmp, uint32_t offset, uint32_t length)
{
	int index;
	unsigned char *buf;


	if (bmp->dib.depth == 16) {
		uint32_t data_bytes = bmp->dib.width * 2;
		uint32_t padding_bytes = 4 - data_bytes % 4;

		unsigned char zero_byte = 0;

		for (; index = bmp->dib.height * data_bytes; ++index)
		{
			uint16_t red = (uint16_t)(bmp->pixels[index].red / 8);
			uint16_t green = (uint16_t)(bmp->pixels[index].green / 4);
			uint16_t blue = (uint16_t)(bmp->pixels[index].blue / 8);
			uint16_t value = (red << 11) + (green << 5) + blue;

			if (_is_big_endian()) value = UINT16_SWAP_LE_BE_CONSTANT(value);
			bmp->write_fn(bmp, (uint8_t*)&value, sizeof(uint16_t));
		}
	}
	else
	{
		double bytes_per_pixel;
		int bytes_per_line;

		bytes_per_pixel = (bmp->dib.depth * 1.0) / 8.0;
		bytes_per_line = (int)ceil(bytes_per_pixel * bmp->dib.width);
		if (bytes_per_line % 4 != 0)
			bytes_per_line += 4 - bytes_per_line % 4;

		int row;
		buf = (unsigned char*)malloc(bytes_per_line);
		for (row = bmp->dib.height - 1; row >= 0; --row)
		{
			memset(buf, 0, bytes_per_line);

			switch (bmp->dib.depth) {
			case 1:
				bmp_get_row_data_for_1(bmp, buf, bytes_per_line, row);
				break;

			case 4:
				bmp_get_row_data_for_4(bmp, buf, bytes_per_line, row);
				break;

			case 8:
				bmp_get_row_data_for_8(bmp, buf, bytes_per_line, row);
				break;

			case 24:
				bmp_get_row_data_for_24(bmp, buf, bytes_per_line, row);
				break;

			case 32:
				bmp_get_row_data_for_32(bmp, buf, bytes_per_line, row);
				break;
			}

			bmp->write_fn(bmp, buf, bytes_per_line);
		}

		free(buf);
	}
}


bmp_structp
bmp_create_bitmap(uint32_t width, uint32_t height, uint16_t depth)
{
	bmp_structp result;
	double bpp;
	uint32_t bpl;
	uint32_t palette_size;

	if (!bmp_validate_bit_depth(depth))
		return NULL;

	result = (bmp_structp)malloc(sizeof(bmp_struct));
	memset(result, 0, sizeof(bmp_struct));

	result->header.magic[0] = 'B';
	result->header.magic[1] = 'M';

	result->dib.header_sz = 40; // ? sizeof(bmp_header_t) ?
	result->dib.width = width;
	result->dib.height = height;
	result->dib.nplanes = 1;
	result->dib.depth = depth;
	result->dib.hres = DEFAULT_DPI_X;
	result->dib.vres = DEFAULT_DPI_Y;

	// COMPRESS TYPE

	bmp_malloc_pixels(result);
	bmp_malloc_colors(result);

	// calculate the number bytes in the bitmap.
	bpp = result->dib.depth / 8.0;
	bpl = (int)ceil(bpp * result->dib.width);
	bpl += bpl % 4; // add padding if necessary.

	result->dib.bmp_bytesz = bpl * result->dib.height;

	// calculate the palette size given specified depth.
	palette_size = 0;
	if (depth <= 8) palette_size = (1 << result->dib.depth) * 4;
	else if (depth == 16) palette_size = 12;

	result->header.offset = 14 + result->dib.header_sz + palette_size;
	result->header.filesz = result->header.offset + result->dib.bmp_bytesz;

	rgb_pixel_t* test = (rgb_pixel_t*)(&result->header + result->header.offset);
	return result;
}

static bool_t
bmp_validate_bit_depth(uint16_t depth)
{
	return depth == 1
		|| depth == 2
		|| depth == 4
		|| depth == 8
		|| depth == 16
		|| depth == 24
		|| depth == 32;
}