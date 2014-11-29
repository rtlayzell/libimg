#ifndef HEADER_NS_BITMAP_ITERATOR_HPP
#define HEADER_NS_BITMAP_ITERATOR_HPP

#pragma once

#include <iterator>


namespace libimg
{
	enum class bitmap_format : short;

	class bitmap_iterator
	{
	private:
		unsigned char* _dataPtr;
		bitmap_format _bmpFmt;

		typedef bitmap_iterator _Myt;
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef unsigned char value_type;
		typedef unsigned char* pointer;
		typedef unsigned char& reference;
		typedef std::ptrdiff_t difference_type;

		bitmap_iterator() = default;
		bitmap_iterator(bitmap_iterator&&) = default;
		bitmap_iterator(bitmap_iterator const&) = default;

		bitmap_iterator(unsigned char* _DataPtr, bitmap_format _Fmt);

		bitmap_iterator& operator = (bitmap_iterator&&) = default;
		bitmap_iterator& operator = (bitmap_iterator const&) = default;

		unsigned int& operator [](std::size_t _Offset) noexcept;
		unsigned int const& operator [](std::size_t _Offset) const noexcept;

		unsigned int& operator *() noexcept;
		unsigned int const& operator *() const noexcept;

		_Myt& operator ++ (int) noexcept;
		_Myt& operator -- (int) noexcept;
		_Myt& operator ++ () noexcept;
		_Myt& operator -- () noexcept;

		_Myt& operator += (difference_type) noexcept;
		_Myt& operator -= (difference_type) noexcept;
	
		friend std::ptrdiff_t operator - (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
		friend bool operator == (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
		friend bool operator < (bitmap_iterator const& _Left, bitmap_iterator const& _Right);
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
}

#endif /*HEADER_NS_BITMAP_ITERATOR_HPP*/