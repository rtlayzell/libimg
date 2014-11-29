#ifndef HEADER_NS_BMP_BITMAP_IMPL_HPP
#define HEADER_NS_BMP_BITMAP_IMPL_HPP

#include <istream>
#include <ostream>
#include "bitmap_impl.hpp"

namespace libimg
{
	struct libimg::bitmap::bitmap_impl::bmp : public libimg::bitmap::bitmap_impl
	{
		bitmap::pointer data() noexcept override { return nullptr; }
		bitmap::const_pointer data() const noexcept override { return nullptr; }

		void load(std::istream&) override { }
		void save(std::ostream&) override { }
	};
}

#endif /*HEADER_NS_BMP_BITMAP_IMPL_HPP*/