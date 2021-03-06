#ifndef HEADER_NS_BITMAP_IMPL_HPP
#define HEADER_NS_BITMAP_IMPL_HPP

#include "..\bitmap.hpp"

namespace libimg
{
	struct bitmap::bitmap_impl
	{
		struct bmp;
		struct png;
		struct jpg;
		struct tif;

		virtual ~bitmap_impl() {}

		virtual void init(std::size_t _Width, std::size_t _Height, pixel_format _Fmt) = 0;
		virtual void load(std::istream&) = 0;
		virtual void save(std::ostream&) = 0;

		virtual bitmap::pointer data() noexcept = 0;
		virtual bitmap::const_pointer data() const noexcept = 0;

		virtual pixel_format format() const noexcept = 0;
		virtual std::size_t xdpi() const noexcept = 0;
		virtual std::size_t ydpi() const noexcept = 0;
		virtual std::size_t width() const noexcept = 0;
		virtual std::size_t height() const noexcept = 0;
		virtual std::size_t size() const noexcept = 0;
	};
}

#endif /*HEADER_NS_PNG_BITMAP_IMPL_HPP*/
