#include "..\..\include\libbmp\bmp.h"
#include "..\..\include\libbmp\bmpdef.h"


void bmp_destroy_read_struct(bmp_structp bmp)
{
	bmp_destroy_struct(bmp);
}

bmp_structp bmp_create_read_struct()
{
	return bmp_create_struct();
}

bool_t bmp_set_read_fn(bmp_structp bmp, void* io_ptr, bmp_rw_ptr fn)
{
	if (fn == NULL)
		return FALSE;

	bmp->read_fn = fn;
	bmp->io_ptr = io_ptr; // maybe null if not needed..

	if (bmp->write_fn != NULL)
	{
		// error: can't have both read and write in the same structure.
		bmp->write_fn = NULL;
	}

	return TRUE;
}


void
bmp_read_header(bmp_structp bmp)
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

		bmp->read_fn(bmp, (uint8_t*)&red_mask, sizeof(uint16_t));
		bmp->read_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));

		bmp->read_fn(bmp, (uint8_t*)&green_mask, sizeof(uint16_t));
		bmp->read_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));

		bmp->read_fn(bmp, (uint8_t*)&blue_mask, sizeof(uint16_t));
		bmp->read_fn(bmp, (uint8_t*)&zero_word, sizeof(uint16_t));
	}
}


void
bmp_read_pixels(bmp_structp bmp, uint32_t offset, uint32_t length)
{
	/*int index;
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
	}*/
}
