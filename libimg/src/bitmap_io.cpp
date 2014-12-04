#include <stdafx.h>
#include <iostream>
#include <iomanip>

#include "bitmap.hpp"
#include "bitmap_io.hpp"

namespace libimg
{
	namespace detail
	{
		_BmpManip::_BmpManip(libimg::pixel_format _Fmt)
			: std::_Smanip<_BmpManip const&>(&_BmpManipFn, *this), _fmt(_Fmt) {}

		_PngManip::_PngManip(pixel_format _Fmt, bool _Interlaced)
			: std::_Smanip<_PngManip const&>(&_PngManipFn, *this), _fmt(_Fmt), _interlaced(_Interlaced) {}

		_JpgManip::_JpgManip(libimg::pixel_format _Fmt)
			: std::_Smanip<_JpgManip const&>(&_JpgManipFn, *this), _fmt(_Fmt) {}

		_TifManip::_TifManip(libimg::pixel_format _Fmt)
			: std::_Smanip<_TifManip const&>(&_TifManipFn, *this), _fmt(_Fmt) {}

		void _BmpManip::_BmpManipFn(std::ios_base& _Ios, _BmpManip const& _Manip)
		{
			// do something like set a flag...
		}

		void _PngManip::_PngManipFn(std::ios_base& _Ios, _PngManip const& _Manip)
		{
			// do something like set a flag...
		}

		void _JpgManip::_JpgManipFn(std::ios_base& _Ios, _JpgManip const& _Manip)
		{
			// do something like set a flag...
		}

		void _TifManip::_TifManipFn(std::ios_base& _Ios, _TifManip const& _Manip)
		{
			// do something like set a flag...
		}
	}

	detail::_BmpManip bmp(libimg::pixel_format _Fmt)
	{
		// TODO: implement a bmp manipulator with specified format.
		return detail::_BmpManip(_Fmt);
	}

	std::ios_base& bmp(std::ios_base& _Ios)
	{
		std::ostream* pos = dynamic_cast<std::ostream*>(&_Ios);
		if (pos) return (*pos) << bmp(libimg::pixel_format::rgb | libimg::pixel_format::bpp24);

		std::istream* pis = dynamic_cast<std::istream*>(&_Ios);
		if (pis) return (*pis) >> bmp(libimg::pixel_format::rgb | libimg::pixel_format::bpp24);

		return _Ios;
	}

	detail::_PngManip png(libimg::pixel_format _Fmt, bool _Interlaced)
	{
		// TODO: implement a png manipulator with specified format.
		return detail::_PngManip(_Fmt, _Interlaced);
	}

	std::ios_base& png(std::ios_base& _Ios)
	{
		std::ostream* pos = dynamic_cast<std::ostream*>(&_Ios);
		if (pos) return (*pos) << png(libimg::pixel_format::rgba | libimg::pixel_format::bpp32, true);

		std::istream* pis = dynamic_cast<std::istream*>(&_Ios);
		if (pis) return (*pis) >> png(libimg::pixel_format::rgba | libimg::pixel_format::bpp32, true);

		return _Ios;
	}

	detail::_JpgManip jpg(libimg::pixel_format _Fmt)
	{
		// TODO: implement a jpg manipulator with specified format.
		return detail::_JpgManip(_Fmt);
	}

	std::ios_base& jpg(std::ios_base& _Ios)
	{
		std::ostream* pos = dynamic_cast<std::ostream*>(&_Ios);
		if (pos) return (*pos) << jpg(libimg::pixel_format::rgb | libimg::pixel_format::bpp24);

		std::istream* pis = dynamic_cast<std::istream*>(&_Ios);
		if (pis) return (*pis) >> jpg(libimg::pixel_format::rgb | libimg::pixel_format::bpp24);

		return _Ios;
	}

	detail::_TifManip tif(libimg::pixel_format _Fmt)
	{
		// TODO: implement a tif manipulator with specified format.
		return detail::_TifManip(_Fmt);
	}

	std::ios_base& tif(std::ios_base& _Ios)
	{
		std::ostream* pos = dynamic_cast<std::ostream*>(&_Ios);
		if (pos) return (*pos) << tif(libimg::pixel_format::rgb | libimg::pixel_format::bpp24);

		std::istream* pis = dynamic_cast<std::istream*>(&_Ios);
		if (pis) return (*pis) >> tif(libimg::pixel_format::rgb | libimg::pixel_format::bpp24);

		return _Ios;
	}


	std::ostream& operator << (std::ostream& _Os, bitmap const& _Bitmap)
	{
		//auto pfunc = reinterpret_cast<std::ostream&(*)(std::ostream&)>(_Os.pword(0));
		//if (pfunc == 0)
		//	_Bitmap._pimpl->save(_Os); // use own pimpl

		// perform the necessary conversion 
		// to the requested bitmap format.
		/*if (pfunc == &libimg::bmp) std::cout << "converting to bmp file format." << std::endl;
		if (pfunc == &libimg::png) std::cout << "converting to png file format." << std::endl;
		if (pfunc == &libimg::tif) std::cout << "converting to tif file format." << std::endl;
		if (pfunc == &libimg::jpg) std::cout << "converting to jpg file format." << std::endl;*/

		return _Os;
	}

	std::istream& operator >> (std::istream& _Is, bitmap& _Bitmap)
	{
		/*_Bitmap._pimpl->load(_Is);
		return _Is;*/

		return _Is;
	}
}