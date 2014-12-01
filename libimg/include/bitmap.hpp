#ifndef HEADER_NS_BITMAP_HPP
#define HEADER_NS_BITMAP_HPP

#pragma once

#ifdef _WIN32
#define _SCL_SECURE_NO_WARNINGS
#endif

#include <ostream>
#include <istream>
#include <fstream>
#include <string>
#include <memory>

#include "bitmap_iterator.hpp"


namespace libimg
{
	// forward declare ostream manipulators for writing
	// the various types of image formats supported by libimg.

	std::ostream& bmp(std::ostream& _Os);
	std::ostream& png(std::ostream& _Os);
	std::ostream& jpg(std::ostream& _Os);
	std::ostream& tif(std::ostream& _Os);

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
		// lo-word represents the channel identifier.
		// hi-word represents the number of channels.
		rgb = 0x0301 << 16,		
		rgba = 0x0402 << 16,
		indexed = 0x0108 << 16,
		grayscale = 0x0104 << 16,
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

	class bitmap
	{
	private:
		struct bitmap_impl;
		std::shared_ptr<bitmap_impl> _pimpl;

		static std::shared_ptr<bitmap::bitmap_impl> _CreateBitmapImpl(std::string _Path);

	public:
		typedef unsigned int value_type;
		typedef unsigned char* pointer;
		typedef value_type& reference;
		typedef unsigned char const* const_pointer;
		typedef value_type const& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef _bitmap_iterator iterator;
		typedef _bitmap_iterator const_iterator;
		typedef std::reverse_iterator<_bitmap_iterator> reverse_iterator;
		typedef std::reverse_iterator<_bitmap_iterator> const_reverse_iterator;
		
		bitmap() = default;
		bitmap(bitmap&&) = default;
		bitmap(bitmap const&);

		explicit bitmap(std::string);
		explicit bitmap(std::istream&);
		bitmap(std::size_t _Width, std::size_t _Height, pixel_format _Fmt = pixel_format::bpp24);
		bitmap(std::size_t _Width, std::size_t _Height, value_type _Color, pixel_format _Fmt = pixel_format::bpp24);

		iterator operator [](std::size_t _Row) noexcept;
		const_iterator operator [](std::size_t _Row) const noexcept;

		bitmap& operator = (bitmap&&) = default;
		bitmap& operator = (bitmap const&);

		iterator pixels() noexcept;
		const_iterator pixels() const noexcept;

		pointer raw() noexcept;
		const_pointer raw() const noexcept;

		bool empty() const noexcept;
		size_type size() const noexcept;
		
		unsigned short depth() const noexcept;
		unsigned short channels() const noexcept;
		pixel_format format() const noexcept;

		size_type dpi() const noexcept;
		size_type xdpi() const noexcept;
		size_type ydpi() const noexcept;

		size_type width() const noexcept;
		size_type height() const noexcept;

		void clear(unsigned int = 0xFF << 24) noexcept;
		void resize(std::size_t _Width, std::size_t _Height);

		void assign(bitmap&&);
		void assign(bitmap const&);

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		reverse_iterator rbegin() noexcept;
		reverse_iterator rend() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator rend() const noexcept;
		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		void swap(bitmap&);

		void read(std::string);
		void write(std::string);

		void read(std::istream&);
		void write(std::ostream&);

		friend std::ostream& operator << (std::ostream& _Os, bitmap const& _Bitmap);
		friend std::istream& operator >> (std::istream& _Is, bitmap& _Bitmap);
	};

	bool operator == (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
	bool operator != (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
	bool operator <= (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
	bool operator >= (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
	bool operator < (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
	bool operator > (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
	
	std::ptrdiff_t operator - (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
	_bitmap_iterator operator + (_bitmap_iterator const& _Left, std::ptrdiff_t _Right);
	_bitmap_iterator operator - (_bitmap_iterator const& _Left, std::ptrdiff_t _Right);
	_bitmap_iterator operator + (std::ptrdiff_t _Left, _bitmap_iterator const& _Right);

	bool operator == (bitmap const& _Left, bitmap const& _Right);
	bool operator != (bitmap const& _Left, bitmap const& _Right);

	bool operator == (bitmap const& _Left, ::std::nullptr_t const);
	bool operator != (bitmap const& _Left, ::std::nullptr_t const);

	bool operator == (::std::nullptr_t const, bitmap const& _Right);
	bool operator != (::std::nullptr_t const, bitmap const& _Right);

	std::ostream& operator << (std::ostream& _Os, bitmap const& _Bitmap);
	std::istream& operator >> (std::istream& _Is, bitmap& _Bitmap);

	template <typename _Elem, typename _Traits>
	inline std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>& _Os, pixel_format _BitDepth) 
	{ return _Os << ((short)_BitDepth); }
}

#endif /*HEADER_NS_BITMAP_HPP*/
