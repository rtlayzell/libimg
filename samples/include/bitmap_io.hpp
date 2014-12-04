#ifndef HEADER_BITMAP_IO_HPP
#define HEADER_BITMAP_IO_HPP

#include <ios>
#include <iomanip>

namespace libimg
{
	// forward declarations
	enum class pixel_format : unsigned int;

	namespace detail
	{
#ifdef _WIN32
		// smanip is platform specific.
		template <typename _T> using smanip = std::_Smanip<_T>;
#endif 

		// An implementation of smanip for BMP stream manipulation.
		struct _BmpManip : smanip<_BmpManip const&> 
		{
		private:
			static void __cdecl _BmpManipFn(std::ios_base&, _BmpManip const&);
			pixel_format _fmt;

		public:
			_BmpManip(pixel_format _Fmt);
		};


		// An implementation of smanip for PNG stream manipulation.
		struct _PngManip : smanip<_PngManip const&> 
		{
		private:
			static void __cdecl _PngManipFn(std::ios_base&, _PngManip const&);
			libimg::pixel_format _fmt;
			bool _interlaced;

		public:
			_PngManip(pixel_format _Fmt, bool _Interlaced);
		};

		// An implementation of smanip for JPG, JPEG stream manipulation.
		struct _JpgManip : smanip<_JpgManip const&>
		{
		private:
			static void __cdecl _JpgManipFn(std::ios_base&, _JpgManip const&);
			pixel_format _fmt;

		public:
			_JpgManip(pixel_format _Fmt);
		};

		// An implementation of smanip for TIF, TIFF stream manipulation.
		struct _TifManip : smanip<_TifManip const&>
		{
		private:
			static void __cdecl _TifManipFn(std::ios_base&, _TifManip const&);
			pixel_format _fmt;

		public:
			_TifManip(pixel_format _Fmt);
		};
	}

	std::ios_base& bmp(std::ios_base& _Ios);
	std::ios_base& png(std::ios_base& _Ios);
	std::ios_base& jpg(std::ios_base& _Os);
	std::ios_base& tif(std::ios_base& _Os);

	detail::_BmpManip bmp(pixel_format _Fmt);
	detail::_PngManip png(pixel_format _Fmt, bool _Interlaced = true);
	detail::_JpgManip jpg(pixel_format _Fmt); // specific options for jpeg.
	detail::_TifManip tif(pixel_format _Fmt); // specific options for tiff.
}

#endif /*HEADER_BITMAP_IO_HPP*/