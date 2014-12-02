#include "stdafx.h"
#include "pixel_format.hpp"

namespace libimg
{
	unsigned int bpp(pixel_format const _Fmt)
	{
		return ((unsigned int)_Fmt & 0xff00) >> 8;
	}

	unsigned int channels(pixel_format const _Fmt)
	{
		return ((unsigned int)_Fmt & 0xf0000000) >> 28;
	}

	pixel_format operator & (pixel_format const _Left, pixel_format const _Right)
	{
		unsigned int lldword = ((unsigned int)_Left  & 0x000000ff);
		unsigned int lhdword = ((unsigned int)_Left  & 0x0fff0000);
		unsigned int rldword = ((unsigned int)_Right & 0x000000ff);
		unsigned int rhdword = ((unsigned int)_Right & 0x0fff0000);

		unsigned int rlo = (lldword & rldword);
		unsigned int rhi = (lhdword & rhdword);

		return (pixel_format)((rlo | rhi)
			| (rlo ? ((unsigned int)_Left & 0x0000ff00) : 0x00)
			| (rhi ? ((unsigned int)_Left & 0xf0000000) : 0x00));
	}

	pixel_format operator | (pixel_format const _Left, pixel_format const _Right)
	{
		unsigned int left = (unsigned int)_Left;
		unsigned int right = (unsigned int)_Right;
		unsigned int result = 0;

		result |= (left & 0x000000ff) | (right & 0x000000ff);
		result |= (((left & 0x0000ff00) >> 8) + ((right & 0x0000ff00) >> 8)) << 8;
		result |= (((left & 0x0fff0000) >> 16) | ((right & 0x0fff0000) >> 16)) << 16;
		result |= (((left & 0xf0000000) >> 28) + ((right & 0xf0000000) >> 28)) << 28;

		return (pixel_format)result;
	}

	pixel_format operator ^ (pixel_format const _Left, pixel_format const _Right)
	{
		// doesn't make sense but whatever...
		return (pixel_format)(
			(unsigned int)_Left ^
			(unsigned int)_Right);
	}

	pixel_format operator &=(pixel_format& _Left, pixel_format const _Right)
	{
		return _Left = _Left & _Right;
	}

	pixel_format operator |= (pixel_format& _Left, pixel_format const _Right)
	{
		return _Left = _Left | _Right;
	}

	pixel_format operator ^= (pixel_format& _Left, pixel_format const _Right)
	{
		return _Left = _Left ^ _Right;
	}

	pixel_format operator ~(pixel_format& _Fmt)
	{
		return (pixel_format)(~(unsigned int)_Fmt);
	}
}