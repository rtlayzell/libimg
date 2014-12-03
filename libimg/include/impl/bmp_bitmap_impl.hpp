#ifndef HEADER_NS_BMP_BITMAP_IMPL_HPP
#define HEADER_NS_BMP_BITMAP_IMPL_HPP

#include <istream>
#include <ostream>
#include <cassert>
#include "bitmap_impl.hpp"
#include "libbmp\bmp.h"

#define BMPSIGSIZE 2;

namespace libimg
{
	static void _IStreamReadBMPData(bmp_structp bmpPtr, uint8_t* data, uint32_t length)
	{
		void* a = bmp_get_io_ptr(bmpPtr);
		((std::istream*)a)->read((char*)data, length);
	}

	static void _OStreamWriteBMPData(bmp_structp bmpPtr, uint8_t* data, uint32_t length)
	{
		// stub..
	}

	struct bitmap::bitmap_impl::bmp : public bitmap::bitmap_impl
	{
	private:
		bmp_structp _bmpp;

	public:
		virtual ~bmp() { 
			bmp_destroy_struct(_bmpp);
		}

		void init(std::size_t _Width, std::size_t _Height, pixel_format _Fmt) override
		{
			if (_bmpp != nullptr)
				bmp_destroy_struct(_bmpp);

			_bmpp = bmp_create_bitmap(_Width, _Height, bpp(_Fmt));
			if (_bmpp == NULL)
				throw std::runtime_error("failed to create BMP data.\n");
		}

		void load(std::istream& _Is) override
		{
			if (_bmpp == nullptr)
				_bmpp = bmp_create_read_struct();

			if (_bmpp == NULL)
				throw std::runtime_error("input is not valid BMP data.\n");


			bmp_set_read_fn(_bmpp, (void*)&_Is, _IStreamReadBMPData);
			
			bmp_read_header(_bmpp);
			bmp_read_dib(_bmpp);
			bmp_read_palette(_bmpp);
		}

		void save(std::ostream& _Os) override
		{
			if (_bmpp == nullptr)
				_bmpp = bmp_create_write_struct();

			bmp_set_write_fn(_bmpp, (void*)&_Os, _OStreamWriteBMPData);

			//bmp_write_header(_bmpp);
			//bmp_write_dib(_bmpp);
			//bmp_write_palette(_bmpp);
		}

		bitmap::pointer data() noexcept override
		{
			return nullptr;
		}

		bitmap::const_pointer data() const noexcept override
		{
			return nullptr;
		}

		pixel_format format() const noexcept override
		{
			uint16_t depth = bmp_get_depth(_bmpp);
			if (depth == 32) return pixel_format::rgba | pixel_format::bpp32;
			else if (depth == 24) return pixel_format::rgb | pixel_format::bpp24;
			else if (depth == 16) return pixel_format::rgb | pixel_format::bpp16;
			else if (depth == 8) return pixel_format::grayscale | pixel_format::bpp8;
			else if (depth == 4) return pixel_format::grayscale | pixel_format::bpp4;
			else if (depth == 1) return pixel_format::bilevel;

			return pixel_format::undefined;
		}

		std::size_t xdpi() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_dpi_y(_bmpp));
		}

		std::size_t ydpi() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_dpi_x(_bmpp));
		}

		std::size_t width() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_width(_bmpp));
		}

		std::size_t height() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_height(_bmpp));
		}

		std::size_t size() const noexcept override 
		{
			return static_cast<std::size_t>(
				bmp_get_filesz(_bmpp));
		}

	};
}

#endif /*HEADER_NS_BMP_BITMAP_IMPL_HPP*/