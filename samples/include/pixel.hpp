#ifndef HEADER_PIXEL_HPP
#define HEADER_PIXEL_HPP

#include <ostream>
#include <istream>
#include <type_traits>
#include <functional>

#include "pixel_format.hpp"

namespace libimg
{
	namespace detail
	{
		template <typename _T>
		struct _Pixel;
	}

	typedef detail::_Pixel<unsigned int> pixel_t;
	typedef detail::_Pixel<unsigned int*> pixelptr_t;
	typedef detail::_Pixel<unsigned int&> pixelref_t;

	namespace detail
	{
		template <typename _T>
		struct _PixelBase
		{
		protected:
			_T _val;
			const pixel_format _fmt;

			template <typename _U>
			friend struct _PixelBase;

			template <typename _U>
			friend struct _Pixel;

			template <typename _T1, typename _T2>
			friend bool operator == (_PixelBase<_T1> const&, _PixelBase<_T2> const&) noexcept;

			template <typename _T1, typename _T2>
			friend bool operator < (_PixelBase<_T1> const&, _PixelBase<_T2> const&) noexcept;

			template <typename _T, typename _Elem, typename _Traits>
			friend std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>&, _PixelBase<_T> const&);

			template <typename _T, typename _Elem, typename _Traits>
			friend std::basic_istream<_Elem, _Traits>& operator << (std::basic_istream<_Elem, _Traits>&, _PixelBase<_T> const&);

			template <typename _T1, typename _T2, typename _Fn2>
			friend void _Transform(_PixelBase<_T1>& _Val1, _PixelBase<_T2> const& _Val2, _Fn2 Func) noexcept;

			template <typename _T1, typename _T2, typename _Fn2>
			friend void _TransformScalar(_PixelBase<_T1>& _Val1, _T2 const& _Val2, _Fn2 _Func) noexcept;

		public:
			_PixelBase() = default;
			_PixelBase(_T _Val, pixel_format _Fmt) : _val(_Val), _fmt(_Fmt) { 
				// process the value with _TransformScalar method, this ensures that 
				// each channel and the number of bits per channel are adhere with overflow.
				_TransformScalar(*this, 0, [](auto x, auto y) { return x; });
			}

			_PixelBase(_PixelBase const& _Other) : _val(_Other._val), _fmt(_Other._fmt) { }

			_PixelBase& operator = (_PixelBase<_T> const& _Other) noexcept {
				// should not be changing the format here.
				// instead convert the _Other pixel to the existing format and store that.
				_val = _Other._val;
				//_fmt = _Other._fmt;
				return *this;
			}

			_PixelBase& operator = (std::remove_reference_t<_T> const _Val) noexcept {
				// process the value with _TransformScalar method, this ensures that 
				// each channel and the number of bits per channel are adhere with overflow.
				_TransformScalar(*this, 0, [](auto x, auto y) { return x; })
				return *this;
			}

			template <typename _U> _PixelBase& operator += (_PixelBase<_U> const& _Other) { _Transform(*this, _Other, std::plus<>()); return *this; }
			template <typename _U> _PixelBase& operator -= (_PixelBase<_U> const& _Other) { _Transform(*this, _Other, std::minus<>()); return *this; }
			template <typename _U> _PixelBase& operator *= (_PixelBase<_U> const& _Other) { _Transform(*this, _Other, std::multiplies<>()); return *this; }
			template <typename _U> _PixelBase& operator /= (_PixelBase<_U> const& _Other) { _Transform(*this, _Other, std::divides<>()); return *this; }
			_PixelBase& operator += (std::remove_reference_t<_T> _Val) { return *this += _PixelBase<_T>(_Val, this->_fmt); }
			_PixelBase& operator -= (std::remove_reference_t<_T> _Val) { return *this -= _PixelBase<_T>(_Val, this->_fmt); }
			_PixelBase& operator *= (std::remove_reference_t<_T> _Val) { _TransformScalar(*this, _Val, std::multiplies<>()); return *this; }
			_PixelBase& operator /= (std::remove_reference_t<_T> _Val) { _TransformScalar(*this, _Val, std::divides<>()); return *this; }

			_Pixel<_T> operator ~ () noexcept { return _Pixel<_T>(~_val, _fmt); }
			template <typename _U> _PixelBase& operator &= (_PixelBase<_U> const& _Other) noexcept { this->_val &= _Other._val; return *this; }
			template <typename _U> _PixelBase& operator |= (_PixelBase<_U> const& _Other) noexcept { this->_val |= _Other._val; return *this; }
			template <typename _U> _PixelBase& operator ^= (_PixelBase<_U> const& _Other) noexcept { this->_val ^= _Other._val; return *this; }
			_PixelBase<_T>& operator &= (std::remove_reference_t<std::remove_pointer_t<_T>> _Val) noexcept { this->_val &= _Val; return *this; }
			_PixelBase<_T>& operator |= (std::remove_reference_t<std::remove_pointer_t<_T>> _Val) noexcept { this->_val |= _Val; return *this; }
			_PixelBase<_T>& operator ^= (std::remove_reference_t<std::remove_pointer_t<_T>> _Val) noexcept { this->_val ^= _Val; return *this; }

			explicit operator _T() const noexcept
			{
				return _val;
			}
		};

		template <typename _T>
		struct _Pixel : _PixelBase<_T>
		{
		public:
			using _PixelBase<_T>::_PixelBase;
			_Pixel(_PixelBase<_T> const& _Other) : _PixelBase(_Other._val, _Other._fmt) {}
			_Pixel(_PixelBase<_T&> const& _Ref) : _PixelBase(_Ref._val, _Ref._fmt) {}

			// not sure why this is actually needed, the base class already provides it,
			// but without this method an error explaining that it cannot be found occurs.
			_Pixel& operator = (std::remove_reference_t<_T> const _Val) noexcept {
				// process the value with _TransformScalar method, this ensures that 
				// each channel and the number of bits per channel are adhere with overflow.
				_TransformScalar(*this, 0, [](auto x, auto y) { return x; });
				return *this;

			}
			_Pixel<_T*> operator & () noexcept { return _Pixel<_T*>(&_val, _fmt); }
			_Pixel<_T*> const operator & () const noexcept { return _Pixel<_T*>(const_cast<_T*>(&_val, _fmt)); }
		};


		template <typename _T>
		struct _Pixel<_T&> : _PixelBase<_T&>
		{
		public:
			using _PixelBase<_T&>::_PixelBase;
			_Pixel(_PixelBase<_T>& _Other) : _PixelBase(_Other._val, _Other._fmt) {}

			// not sure why this is actually needed, the base class already provides it,
			// but without this method an error explaining that it cannot be found occurs.
			_Pixel& operator = (std::remove_reference_t<_T> const _Val) noexcept {
				// process the value with _TransformScalar method, this ensures that 
				// each channel and the number of bits per channel are adhere with overflow.
				_TransformScalar(*this, 0, [](auto x, auto y) { return x; });
				return *this;
			}

			_Pixel<_T*> operator & () noexcept { return _Pixel<_T*>(&_val, _fmt); }
			_Pixel<_T*> const operator & () const noexcept { return _Pixel<_T*>(const_cast<_T*>(&_val, _fmt)); }
		};


		template <typename _T>
		struct _Pixel<_T*> : _PixelBase<_T*>
		{
		public:
			using _PixelBase<_T*>::_PixelBase;
			_Pixel<_T&> operator * () noexcept { return _Pixel<_T&>(*_val, _fmt); }
			_Pixel<_T&> const operator * () const noexcept { return _Pixel<_T&>(*_val, _fmt); }
		};

		template <typename _T, typename _Elem, typename _Traits>
		std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>& _Os, _PixelBase<_T> const& _Val)
		{
			std::ios init(NULL);
			init.copyfmt(_Os);

			_Os << std::hex;
			if (_Os.flags() & std::ios::showbase)
				_Os << std::noshowbase << '#';

			_Os << std::setfill('0')
				<< std::setw(bpp(_Val._fmt) / 4)
				<< ((_T)_Val);

			_Os.copyfmt(init);
			return _Os;
		}

		template <typename _T, typename _Elem, typename _Traits>
		std::basic_istream<_Elem, _Traits>& operator << (std::basic_istream<_Elem, _Traits>& _Is, _PixelBase<_T> const& _Val)
		{
			return _Is << std::hex << ((_T)_Val);
		}


		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator + (
			_Pixel<_T1> const& _Left,
			_Pixel<_T2> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) += _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator - (
			_Pixel<_T1> const& _Left,
			_Pixel<_T2> const& _Right) noexcept
		{
			return _PixelBase<std::remove_reference_t<_T1>>(_Left) -= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator * (
			_Pixel<_T1> const& _Left,
			_Pixel<_T2> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) *= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator / (
			_Pixel<_T1> const& _Left,
			_Pixel<_T2> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) /= _Right;
		}

		template <typename _T>
		_Pixel<std::remove_reference_t<_T>> operator + (
			_Pixel<_T> const& _Left, std::remove_reference_t<_T> _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T>>(_Left) += _Right;
		}

		template <typename _T>
		_Pixel<std::remove_reference_t<_T>> operator - (
			std::remove_reference_t<_T> _Left, _Pixel<_T> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T>>(_Right) -= _Left;
		}

		template <typename _T>
		_Pixel<std::remove_reference_t<_T>> operator * (
			_Pixel<_T> const& _Left, std::remove_reference_t<_T> _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T>>(_Left) *= _Right;
		}

		template <typename _T>
		_Pixel<std::remove_reference_t<_T>> operator * (
			std::remove_reference_t<_T> _Left, _Pixel<_T> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T>>(_Right) *= _Left;
		}

		template <typename _T>
		_Pixel<std::remove_reference_t<_T>> operator / (
			_Pixel<_T> const& _Left, std::remove_reference_t<_T> _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T>>(_Left) /= _Right;
		}


		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator & (
			_Pixel<_T1> const& _Left,
			_Pixel<_T2> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) &= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator | (
			_Pixel<_T1> const& _Left,
			_Pixel<_T2> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) |= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator ^ (
			_Pixel<_T1> const& _Left,
			_Pixel<_T2> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) ^= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator & (
			_Pixel<_T1> const& _Left,
			std::remove_reference_t<std::remove_pointer_t<_T1>> _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) &= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator | (
			_Pixel<_T1> const& _Left,
			std::remove_reference_t<std::remove_pointer_t<_T1>> _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) |= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator ^ (
			_Pixel<_T1> const& _Left,
			std::remove_reference_t<std::remove_pointer_t<_T1>> _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Left) ^= _Right;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator & (
			std::remove_reference_t<std::remove_pointer_t<_T1>> _Left,
			_Pixel<_T1> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Right) &= _Left;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator | (
			std::remove_reference_t<std::remove_pointer_t<_T1>> _Left,
			_Pixel<_T1> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Right) |= _Left;
		}

		template <typename _T1, typename _T2>
		_Pixel<std::remove_reference_t<_T1>> operator ^ (
			std::remove_reference_t<std::remove_pointer_t<_T1>> _Left,
			_Pixel<_T1> const& _Right) noexcept
		{
			return _Pixel<std::remove_reference_t<_T1>>(_Right) ^= _Left;
		}

		template <typename _T1, typename _T2>
		bool operator == (_PixelBase<_T1> const& _Left, _PixelBase<_T2> const& _Right) noexcept
		{
			return _Left._val == _Right._val;
		}

		template <typename _T1, typename _T2>
		bool operator != (_PixelBase<_T1> const& _Left, _PixelBase<_T2> const& _Right) noexcept
		{
			return !(_Left == _Right);
		}

		template <typename _T1, typename _T2>
		bool operator <= (_PixelBase<_T1> const& _Left, _PixelBase<_T2> const& _Right) noexcept
		{
			return !(_Right < _Left);
		}

		template <typename _T1, typename _T2>
		bool operator >= (_PixelBase<_T1> const& _Left, _PixelBase<_T2> const& _Right) noexcept
		{
			return !(_Left < _Right)
		}

		template <typename _T1, typename _T2>
		bool operator < (_PixelBase<_T1> const& _Left, _PixelBase<_T2> const& _Right) noexcept
		{
			return _Left._val < _Right._val;
		}

		template <typename _T1, typename _T2>
		bool operator >(_PixelBase<_T1> const& _Left, _PixelBase<_T2> const& _Right) noexcept
		{
			return _Right < _Left;
		}

		template <typename _T1, typename _T2, typename _Fn2>
		void _Transform(_PixelBase<_T1>& _Val1, _PixelBase<_T2> const& _Val2, _Fn2 _Func) noexcept
		{
			const unsigned int chnls = channels(_Val1._fmt);
			const unsigned int chnlbits = bpp(_Val1._fmt) / chnls;
			const unsigned int mask = ~(~1ULL << (chnlbits - 1));

			unsigned int result = 0;
			for (std::size_t i = 0; i < chnls; ++i)
			{
				const unsigned int val1 = (_Val1._val >> (i * chnlbits)) & mask; // shift into the lowest channel.
				const unsigned int val2 = (_Val2._val >> (i * chnlbits)) & mask; // shift into the lowest channel.
				result |= (_Func(val1, val2) & mask) << (i * chnlbits); // perform operation, mask bits from lowest channel, shift back into correct channel.
			}

			_Val1._val = result;
		}

		template <typename _T1, typename _T2, typename _Fn2>
		void _TransformScalar(_PixelBase<_T1>& _Val1, _T2 const& _Val2, _Fn2 _Func) noexcept
		{
			const unsigned int chnls = channels(_Val1._fmt);
			const unsigned int chnlbits = bpp(_Val1._fmt) / chnls;
			const unsigned int mask = ~(~1ULL << (chnlbits - 1));

			unsigned int result = 0;
			for (std::size_t i = 0; i < chnls; ++i)
			{
				const unsigned int val = (_Val1._val >> (i * chnlbits)) & mask;
				result |= ((unsigned int)(_Func(val, _Val2)) & mask) << (i * chnlbits);
			}

			_Val1._val = result;
		}
	}
}


#endif HEADER_PIXEL_HPP