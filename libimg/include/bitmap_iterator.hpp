#ifndef HEADER_NS_BITMAP_ITERATOR_HPP
#define HEADER_NS_BITMAP_ITERATOR_HPP

#pragma once

#include <iterator>


namespace libimg
{
	struct _pixel_t;
	struct _bitmap_iterator;
	enum class pixel_format : unsigned int;
	


	typedef struct _pixel_t
	{
	private:
		friend struct _bitmap_iterator;
		friend bool operator == (_pixel_t const&, _pixel_t const&);
		friend bool operator != (_pixel_t const&, _pixel_t const&);

		unsigned int& _val;
		pixel_format& _fmt;

		_pixel_t(unsigned int& _Val, pixel_format& _Fmt);

	public:
		_pixel_t& operator = (unsigned int);
		_pixel_t& operator = (_pixel_t const&);

		_pixel_t& operator += (unsigned int);
		_pixel_t& operator -= (unsigned int);
		_pixel_t& operator *= (unsigned int);
		_pixel_t& operator /= (unsigned int);

#if (-1UL != -1U)
		operator unsigned long();
		operator signed long();
#endif
		operator unsigned long long() const noexcept;
		operator unsigned int() const noexcept;
		operator unsigned short() const noexcept;
		operator unsigned char() const noexcept;

		operator signed long long() const noexcept;
		operator signed int() const noexcept;
		operator signed short() const noexcept;
		operator signed char() const noexcept;
	} pixel_t;

	bool operator == (_pixel_t const&, _pixel_t const&);
	bool operator != (_pixel_t const&, _pixel_t const&);



	struct _bitmap_iterator
	{
	private:
		unsigned char* _dataPtr;
		pixel_format _bmpFmt;

		typedef _bitmap_iterator _Myt;
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef unsigned char value_type;
		typedef unsigned char* pointer;
		typedef unsigned char& reference;
		typedef std::ptrdiff_t difference_type;

		_bitmap_iterator() = default;
		_bitmap_iterator(_bitmap_iterator&&) = default;
		_bitmap_iterator(_bitmap_iterator const&) = default;

		_bitmap_iterator(unsigned char* _DataPtr, pixel_format _Fmt);

		_bitmap_iterator& operator = (_bitmap_iterator&&) = default;
		_bitmap_iterator& operator = (_bitmap_iterator const&) = default;

		pixel_t operator [](std::size_t _Offset) noexcept;
		pixel_t const operator [](std::size_t _Offset) const noexcept;

		pixel_t operator *() noexcept;
		pixel_t const operator *() const noexcept;

		_Myt& operator ++ (int) noexcept;
		_Myt& operator -- (int) noexcept;
		_Myt operator ++ () noexcept;
		_Myt operator -- () noexcept;

		_Myt& operator += (difference_type) noexcept;
		_Myt& operator -= (difference_type) noexcept;
	
		friend std::ptrdiff_t operator - (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
		friend bool operator == (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
		friend bool operator < (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right);
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
}

#endif /*HEADER_NS_BITMAP_ITERATOR_HPP*/