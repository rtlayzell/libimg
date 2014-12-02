#ifndef HEADER_NS_BITMAP_ITERATOR_HPP
#define HEADER_NS_BITMAP_ITERATOR_HPP

#pragma once

#include <iterator>
#include <type_traits>
#include "pixel_format.hpp"
#include "pixel.hpp"

namespace libimg
{
	struct _bitmap_iterator
	{
	private:
		unsigned char* _dataPtr;
		pixel_format _bmpFmt;

		typedef _bitmap_iterator _Myt;
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef pixel_t value_type;
		typedef pixelptr_t pointer;
		typedef pixelref_t reference;
		typedef pixelptr_t const const_pointer;
		typedef pixelref_t const const_reference;
		typedef std::ptrdiff_t difference_type;

		_bitmap_iterator() = default;
		_bitmap_iterator(_bitmap_iterator&&) = default;
		_bitmap_iterator(_bitmap_iterator const&) = default;

		_bitmap_iterator(unsigned char* _DataPtr, pixel_format _Fmt);

		_bitmap_iterator& operator = (_bitmap_iterator&&) = default;
		_bitmap_iterator& operator = (_bitmap_iterator const&) = default;

		reference operator [](std::size_t _Offset) noexcept;
		const_reference operator [](std::size_t _Offset) const noexcept;
		
		reference operator *() noexcept;
		const_reference operator *() const noexcept;

		_Myt& operator ++ (int) noexcept;
		_Myt& operator -- (int) noexcept;
		_Myt operator ++ () noexcept;
		_Myt operator -- () noexcept;

		_Myt& operator += (difference_type) noexcept;
		_Myt& operator -= (difference_type) noexcept;
	
		friend std::ptrdiff_t operator - (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
		friend bool operator == (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
		friend bool operator < (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
	};

	bool operator == (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
	bool operator != (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
	bool operator <= (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
	bool operator >= (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
	bool operator < (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
	bool operator > (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;

	std::ptrdiff_t operator - (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right) noexcept;
	_bitmap_iterator operator + (_bitmap_iterator const& _Left, std::ptrdiff_t _Right) noexcept;
	_bitmap_iterator operator - (_bitmap_iterator const& _Left, std::ptrdiff_t _Right) noexcept;
	_bitmap_iterator operator + (std::ptrdiff_t _Left, _bitmap_iterator const& _Right) noexcept;
}

#endif /*HEADER_NS_BITMAP_ITERATOR_HPP*/