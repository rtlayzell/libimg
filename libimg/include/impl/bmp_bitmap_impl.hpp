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
		bmp_structp _bmpPtr;

	public:
		virtual ~bmp() { 
			bmp_destroy_read_struct(_bmpPtr); 
		}

		void load(std::istream& _Is) override
		{
			if (_bmpPtr == nullptr)
				_bmpPtr = bmp_create_read_struct();

			if (_bmpPtr == NULL)
				throw std::runtime_error("input is not valid BMP data.\n");


			bmp_set_read_fn(_bmpPtr, (void*)&_Is, _IStreamReadBMPData);
			
			bmp_read_header(_bmpPtr);
			bmp_read_dib(_bmpPtr);
			bmp_read_palette(_bmpPtr);

			// testing...
			auto x = bmp_get_dpi_x(_bmpPtr);
			auto y = bmp_get_dpi_y(_bmpPtr);
			assert(x == y && "mismatch in dpi");
		}

		void save(std::ostream& _Os) override
		{
			if (_bmpPtr == nullptr)
				_bmpPtr = bmp_create_write_struct();

			bmp_set_write_fn(_bmpPtr, (void*)&_Os, _OStreamWriteBMPData);

			//bmp_write_header(_bmpPtr);
			//bmp_write_dib(_bmpPtr);
			//bmp_write_palette(_bmpPtr);
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
			uint16_t depth = bmp_get_depth(_bmpPtr);
			/*if (depth == 24) return pixel_format::rgb;
			else if (depth == 16) return pixel_format::rgb;
			else if (depth == 8) return pixel_format::rgb;
			else if (depth == 4) return pixel_format::rgb;
			else if (depth == 1) return pixel_format::gray;*/

			return pixel_format::undefined;
		}

		std::size_t xdpi() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_dpi_y(_bmpPtr));
		}

		std::size_t ydpi() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_dpi_x(_bmpPtr));
		}

		std::size_t width() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_width(_bmpPtr));
		}

		std::size_t height() const noexcept override
		{
			return static_cast<std::size_t>(
				bmp_get_height(_bmpPtr));
		}
	};
}

#endif /*HEADER_NS_BMP_BITMAP_IMPL_HPP*/