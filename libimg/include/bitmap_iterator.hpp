#ifndef HEADER_NS_BITMAP_ITERATOR_HPP
#define HEADER_NS_BITMAP_ITERATOR_HPP

#pragma once

#include <iterator>
#include <type_traits>
#include "pixel_format.hpp"
#include "pixel.hpp"

namespace libimg
{
	namespace detail
	{
		struct _BitmapIterator
		{
		private:
			unsigned char* _dataPtr;

			pixel_format _pixelFmt;	 // describes the number of bits per pixel so we can adjust the pointer appropriately.

			unsigned short _padding; // some formats incorporate padding between pixels to fit some 
									 // byte boundary, this is used to adjust the offset of the pointer.

			typedef _BitmapIterator _Myt;

		public:
			typedef std::random_access_iterator_tag iterator_category;
			typedef pixel_t value_type;
			typedef pixelptr_t pointer;
			typedef pixelref_t reference;
			typedef pixelptr_t const const_pointer;
			typedef pixelref_t const const_reference;
			typedef std::ptrdiff_t difference_type;

			_BitmapIterator() = default;
			_BitmapIterator(_BitmapIterator&&) = default;
			_BitmapIterator(_BitmapIterator const&) = default;

			_BitmapIterator(unsigned char* _DataPtr, pixel_format _Fmt);

			_BitmapIterator& operator = (_BitmapIterator&&) = default;
			_BitmapIterator& operator = (_BitmapIterator const&) = default;

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

			friend std::ptrdiff_t operator - (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
			friend bool operator == (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
			friend bool operator < (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
		};

		bool operator == (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
		bool operator != (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
		bool operator <= (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
		bool operator >= (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
		bool operator < (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
		bool operator > (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;

		std::ptrdiff_t operator - (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept;
		_BitmapIterator operator + (_BitmapIterator const& _Left, std::ptrdiff_t _Right) noexcept;
		_BitmapIterator operator - (_BitmapIterator const& _Left, std::ptrdiff_t _Right) noexcept;
		_BitmapIterator operator + (std::ptrdiff_t _Left, _BitmapIterator const& _Right) noexcept;
	}
}

#endif /*HEADER_NS_BITMAP_ITERATOR_HPP*/