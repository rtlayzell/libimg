#ifndef HEADER_NS_BMP_BITMAP_IMPL_HPP
#define HEADER_NS_BMP_BITMAP_IMPL_HPP

#include <istream>
#include <ostream>
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

	static void _OStreamWriteBMPData()
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
			_bmpPtr = bmp_create_read_struct();
			bmp_set_read_fn(_bmpPtr, (void*)&_Is, _IStreamReadBMPData);
			
			bmp_read_header_info(_bmpPtr);
			bmp_read_dib(_bmpPtr);
			bmp_read_palette(_bmpPtr);
		}

		void save(std::ostream&) override
		{

		}

		bitmap::pointer data() noexcept override
		{
			return nullptr;
		}

		bitmap::const_pointer data() const noexcept override
		{
			return nullptr;
		}

		bitmap_format format() const noexcept override
		{
			return bitmap_format::unknown;
		}

		std::size_t dpi() const noexcept override
		{
			return bmp_get_dpi(_bmpPtr);
		}

		std::size_t width() const noexcept override
		{
			return bmp_get_width(_bmpPtr);
		}

		std::size_t height() const noexcept override
		{
			return bmp_get_height(_bmpPtr);
		}
	};
}

#endif /*HEADER_NS_BMP_BITMAP_IMPL_HPP*/