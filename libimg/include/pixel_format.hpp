#ifndef HEADER_PIXEL_FORMAT_HPP
#define HEADER_PIXEL_FORMAT_HPP

namespace libimg
{
	enum class pixel_format : unsigned int
	{
		undefined = 0x0000,	// ----  | 0x00

		// bits-per-pixel format flags in lo-dword
		// lo-word represents bpp identifier
		// hi-word represents number of bits.
		bpp1 = 0x0101,		// 1bpp  | 0x01
		bpp2 = 0x0202,		// 2bpp  | 0x02
		bpp4 = 0x0404,		// 4bpp  | 0x04
		bpp8 = 0x0808,		// 8bpp  | 0x08
		bpp16 = 0x1010,		// 16bpp | 0x10
		bpp24 = 0x1820,		// 24bpp | 0x20
		bpp32 = 0x2040,		// 32bpp | 0x40

		// channels format flags in hi-dword
		// lo-nibble*3 represents the channel identifier.
		// hi-nibble represents the number of channels.
		rgb = 0x3001 << 16,
		rgba = 0x4002 << 16,
		indexed = 0x1008 << 16,
		grayscale = 0x1004 << 16,
	};

	pixel_format operator & (pixel_format const _Left, pixel_format const _Right);
	pixel_format operator | (pixel_format const _Left, pixel_format const _Right);
	pixel_format operator ^ (pixel_format const _Left, pixel_format const _Right);
	pixel_format operator &=(pixel_format& _Left, pixel_format const _Right);
	pixel_format operator |= (pixel_format& _Left, pixel_format const _Right);
	pixel_format operator ^= (pixel_format& _Left, pixel_format const _Right);
	pixel_format operator ~(pixel_format& _Fmt);


	unsigned int bpp(pixel_format const);
	unsigned int channels(pixel_format const);
}

#endif /*HEADER_PIXEL_FORMAT_HPP*/
