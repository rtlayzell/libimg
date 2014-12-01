#ifndef HEADER_NS_BITMAP_ITERATOR_HPP
#define HEADER_NS_BITMAP_ITERATOR_HPP

#pragma once

#include <iterator>
#include <type_traits>

namespace libimg
{
	struct _bitmap_iterator;
	enum class pixel_format : unsigned int;

	namespace detail
	{
		template <typename _T> typename std::add_lvalue_reference<_T>::type asref(_T * _Ptr) { return *_Ptr; }
		template <typename _T> typename std::add_lvalue_reference<_T>::type asref(_T & _Ref) { return _Ref; }
		template <typename _T> typename std::add_pointer<_T>::type asptr(_T * _Ptr) { return _Ptr; }
		template <typename _T> typename std::add_pointer<_T>::type asptr(_T & _Ref) { return &_Ref; }

		template <typename _T>
		struct _pixel_t
		{
		private:
			_T _val;
			pixel_format _fmt;

			template <typename _U>
			friend struct _pixel_t;

			template <typename _T, typename _Fn2>
			friend void _Transform(_pixel_t<_T>& _Val1, _pixel_t<_T> const& _Val2, _Fn2 Func);

		public:
			_pixel_t() = default;
			_pixel_t(_pixel_t&&) = default;
			_pixel_t(_pixel_t const&) = default;
			_pixel_t(_T _Val, const pixel_format _Fmt) noexcept;

			template <typename _U> 
			_pixel_t(_pixel_t<_U> const& _Other)
				: _val(asref(_Other._val)), _fmt(_Other._fmt)
 			{
			}

			template <typename _U> 
			_pixel_t& operator = (_pixel_t<_U> const& _Other)
			{
				asref(this->_val) = asref(_Other._val);
				this->_fmt = _Other._fmt;

				return *this;
			}

			_pixel_t<_T>& operator = (_pixel_t<_T>&&) = default;
			_pixel_t<_T>& operator = (_pixel_t<_T> const&);

			_pixel_t<_T>& operator = (unsigned int);

			_pixel_t<_T>& operator += (_pixel_t<_T> const&);
			_pixel_t<_T>& operator -= (_pixel_t<_T> const&);
			_pixel_t<_T>& operator *= (_pixel_t<_T> const&);
			_pixel_t<_T>& operator /= (_pixel_t<_T> const&);
			_pixel_t<_T>& operator *= (double const);
			_pixel_t<_T>& operator /= (double const);

			operator unsigned int() const noexcept;

			template <typename _U>
			friend bool operator == (_pixel_t<_U> const&, _pixel_t<_U> const&) noexcept;

			template <typename _U>
			friend bool operator < (_pixel_t<_U> const&, _pixel_t<_U> const&) noexcept;
		};

		template <typename _T> bool operator == (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> bool operator != (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> bool operator <= (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> bool operator >= (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> bool operator < (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> bool operator > (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		
		template <typename _T> _pixel_t<_T> operator + (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> _pixel_t<_T> operator - (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> _pixel_t<_T> operator * (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> _pixel_t<_T> operator / (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept;
		template <typename _T> _pixel_t<_T> operator * (_pixel_t<_T> const&, double const) noexcept;
		template <typename _T> _pixel_t<_T> operator * (double const, _pixel_t<_T> const&) noexcept;
		template <typename _T> _pixel_t<_T> operator / (_pixel_t<_T> const&, double const) noexcept;
	}

	typedef detail::_pixel_t<unsigned int> pixel_t;

	struct _bitmap_iterator
	{
	private:
		unsigned char* _dataPtr;
		pixel_format _bmpFmt;

		typedef _bitmap_iterator _Myt;
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef detail::_pixel_t<unsigned int> value_type;
		typedef detail::_pixel_t<unsigned int*> pointer;
		typedef detail::_pixel_t<unsigned int&> reference;
		typedef detail::_pixel_t<unsigned int*> const const_pointer;
		typedef detail::_pixel_t<unsigned int&> const const_reference;
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