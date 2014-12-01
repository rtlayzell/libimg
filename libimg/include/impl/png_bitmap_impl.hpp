#ifndef HEADER_NS_PNG_BITMAP_IMPL_HPP
#define HEADER_NS_PNG_BITMAP_IMPL_HPP

#include "bitmap_impl.hpp"
#include "libpng\png.h"

#define PNGSIGSIZE 8

namespace libimg
{
	static void _IStreamReadPNGData(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		png_voidp a = png_get_io_ptr(pngPtr);
		((std::istream*)a)->read((char*)data, length);
	}

	static void _OStreamWritePNGData( )
	{
		// stub..
	}

	struct libimg::bitmap::bitmap_impl::png : public libimg::bitmap::bitmap_impl
	{
	private:
		png_structp _pngPtr;
		png_infop _infoPtr;

		bool _Validate(std::istream& _Is)
		{
			png_byte pngsig[PNGSIGSIZE];
			int is_png = 0;

			_Is.read((char*)pngsig, PNGSIGSIZE);
			if (!_Is.good()) return false;

			is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
			return (is_png == 0);
		}

	public:
		virtual ~png()
		{
			png_destroy_read_struct(&_pngPtr, &_infoPtr, (png_infopp)0);
		}

		// could probably do better than this but it'll do for now.
		void load(std::istream& _Is) override
		{
			if (!_Validate(_Is))
				throw std::invalid_argument("input is not valid PNG data.");
			
			
			png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (pngPtr == NULL)
				throw std::runtime_error("could not initialize PNG.");

			png_infop infoPtr = png_create_info_struct(pngPtr);
			if (!infoPtr)
			{
				png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
				throw std::runtime_error("could not initialize PNG.");
			}

			png_bytep* rowPtrs = NULL;
			char* data = NULL;

			if (setjmp(png_jmpbuf(pngPtr)))
			{
				png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);
				if (rowPtrs != NULL) delete [] rowPtrs;
				if (data != NULL) delete [] data;

				throw std::runtime_error("an error occured while reading PNG data.");
			}

			png_set_read_fn(pngPtr, (png_voidp)&_Is, _IStreamReadPNGData);
			png_set_sig_bytes(pngPtr, PNGSIGSIZE);
			png_read_info(pngPtr, infoPtr);
			
			this->_pngPtr = pngPtr;
			this->_infoPtr = infoPtr;
		}

		void save(std::ostream& _Os) override
		{
			_OStreamWritePNGData();
		}

		bitmap::pointer data() noexcept override
		{
			return bitmap::pointer();
		}

		bitmap::const_pointer data() const noexcept override
		{
			return bitmap::const_pointer();
		}

		pixel_format format() const noexcept override
		{
			png_byte color_type = png_get_color_type(this->_pngPtr, this->_infoPtr);
			/*switch (color_type)
			{
			case PNG_COLOR_TYPE_PALETTE: return pixel_format::pallette;
			case PNG_COLOR_TYPE_GRAY: return pixel_format::gray;
			case PNG_COLOR_TYPE_GRAY_ALPHA: return (pixel_format::gray_alpha);
			case PNG_COLOR_TYPE_RGB: return pixel_format::rgb;
			case PNG_COLOR_TYPE_RGBA: return pixel_format::rgba;
			}*/

			return pixel_format::undefined;
		}

		std::size_t xdpi() const noexcept override
		{
			return static_cast<bitmap::size_type>(
				png_get_x_pixels_per_inch(this->_pngPtr, this->_infoPtr));
		}

		std::size_t ydpi() const noexcept override
		{
			return static_cast<bitmap::size_type>(
				png_get_y_pixels_per_inch(this->_pngPtr, this->_infoPtr));
		}

		bitmap::size_type width() const noexcept override
		{
			return static_cast<bitmap::size_type>(
				png_get_image_width(this->_pngPtr, this->_infoPtr));
		}

		bitmap::size_type height() const noexcept override
		{
			return static_cast<bitmap::size_type>(
				png_get_image_height(this->_pngPtr, this->_infoPtr));
		}
	};
}

#endif /*HEADER_NS_PNG_BITMAP_IMPL_HPP*/