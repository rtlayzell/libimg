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
	enum class bitmap_format : short
	{
		unknown = 0x00,
		alpha = 0x01,
		rgb = 0x02,
		bgr = 0x04,
		gray = 0x08,
		gray_alpha = (alpha | gray),
		pallette = 0x10,
		argb = (alpha | rgb),
		abgr = (alpha | bgr),
		rgba = (alpha | rgb),
		bgra = (alpha | rgb),
	};

	enum class bitmap_bitdepth : short
	{
		unknown = 0,
		bit8 = 8,
		bit16 = 16,
		bit24 = 24,
		bit32 = 32,
	};

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

		typedef bitmap_iterator iterator;
		typedef bitmap_iterator const_iterator;
		typedef std::reverse_iterator<bitmap_iterator> reverse_iterator;
		typedef std::reverse_iterator<bitmap_iterator> const_reverse_iterator;
		
		bitmap() = default;
		bitmap(bitmap&&) = default;
		bitmap(bitmap const&);

		explicit bitmap(std::string _Filename);
		bitmap(std::size_t _Width, std::size_t _Height, bitmap_format _Fmt = bitmap_format::argb);
		bitmap(std::size_t _Width, std::size_t _Height, value_type _Color = 0x0, bitmap_format _Fmt = bitmap_format::argb);

		reference operator()(std::size_t _X, std::size_t _Y) noexcept;
		const_reference operator()(std::size_t _X, std::size_t _Y) const noexcept;

		void set_pixel(std::size_t _X, std::size_t _Y, unsigned int _Color);
		unsigned int get_pixel(std::size_t _X, std::size_t _Y) const;

		iterator pixels() noexcept;
		const_iterator pixels() const noexcept;

		pointer raw() noexcept;
		const_pointer raw() const noexcept;

		bool empty() const noexcept;
		bitmap_format format() const noexcept;
		size_type size() const noexcept;
		
		bitmap_bitdepth bit_depth() const noexcept;
		size_type dpi() const noexcept;
		
		size_type width() const noexcept;
		size_type height() const noexcept;

		void clear(unsigned int = 0x0) noexcept;

		void assign(bitmap&&);
		void assign(bitmap const&);
		
		bitmap& operator = (bitmap&&) = default;
		bitmap& operator = (bitmap const&);

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

		friend std::ostream& operator << (std::ostream& _Os, bitmap const& _Bitmap);
		friend std::istream& operator << (std::istream& _Is, bitmap& _Bitmap);
	};

	bool operator == (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
	bool operator != (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
	bool operator <= (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
	bool operator >= (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
	bool operator < (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
	bool operator > (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
	
	std::ptrdiff_t operator - (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
	bitmap_iterator operator + (bitmap_iterator const& _Left, std::ptrdiff_t _Right);
	bitmap_iterator operator - (bitmap_iterator const& _Left, std::ptrdiff_t _Right);
	bitmap_iterator operator + (std::ptrdiff_t _Left, bitmap_iterator const& _Right);

	bool operator == (bitmap const& _Left, bitmap const& _Right);
	bool operator != (bitmap const& _Left, bitmap const& _Right);

	bool operator == (bitmap const& _Left, ::std::nullptr_t const);
	bool operator != (bitmap const& _Left, ::std::nullptr_t const);

	bool operator == (::std::nullptr_t const, bitmap const& _Right);
	bool operator != (::std::nullptr_t const, bitmap const& _Right);

	std::ostream& operator << (std::ostream& _Os, bitmap const& _Bitmap);
	std::istream& operator << (std::istream& _Is, bitmap& _Bitmap);

	template <typename _Elem, typename _Traits>
	inline std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>& _Os, bitmap_bitdepth _BitDepth) 
	{ return _Os << ((short)_BitDepth); }
}

#endif /*HEADER_NS_BITMAP_HPP*/
