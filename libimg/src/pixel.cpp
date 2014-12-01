#include <stdafx.h>
#include <cassert>
#include <functional>

#include "bitmap_iterator.hpp"
#include "bitmap.hpp"

#define FMT_BPP(X) (((std::size_t)X) & 0xFF)

namespace libimg
{
	namespace detail
	{
	
		template <typename _T>
		_pixel_t<_T>::_pixel_t(_T _Val, const pixel_format _Fmt) noexcept
			: _val(_Val), _fmt(_Fmt) {}

		template <typename _T>
		_pixel_t<_T>& _pixel_t<_T>::operator = (_pixel_t<_T> const& _Other)
		{
			asref(this->_val) = asref(_Other._val);
			return *this;
		}

		template <typename _T, typename _Fn2>
		void _Transform(_pixel_t<_T>& _Val1, _pixel_t<_T> const& _Val2, _Fn2 Func)
		{
			const unsigned int chnls = channels(_Val1._fmt);
			const unsigned int chnlbits = bpp(_Val1._fmt) / chnls;
			const unsigned int mask = ~(~1 << (chnlbits - 1));

			unsigned int result = 0;
			for (std::size_t i = 0; i < chnls; ++i)
			{
				const unsigned int val1 = asref(_Val1._val) >> (i*chnlbits); // shift into the lowest channel.
				const unsigned int val2 = asref(_Val2._val) >> (i*chnlbits); // shift into the lowest channel.
				result |= (Func(val1, val2) & mask) << (i * chnlbits); // perform operation, mask bits from lowest channel, shift back into correct channel.
			}

			asref(_Val1._val) = result;
		}

		/*template <typename _T, typename _Fn2>
		void _Transform*/

		template <typename _T>
		_pixel_t<_T>& _pixel_t<_T>::operator += (_pixel_t<_T> const& _Val)
		{
			_Transform(*this, _Val, std::plus<>());
			return *this;
		}

		template <typename _T>
		_pixel_t<_T>& _pixel_t<_T>::operator -= (_pixel_t<_T> const& _Val)
		{
			_Transform(*this, _Val, std::minus<>());
			return *this;
		}

		template <typename _T>
		_pixel_t<_T>& _pixel_t<_T>::operator *= (_pixel_t<_T> const& _Val)
		{
			_Transform(*this, _Val, std::multiplies<>());
			return *this;
		}

		template <typename _T>
		_pixel_t<_T>& _pixel_t<_T>::operator /= (_pixel_t<_T> const& _Val)
		{
			_Transform(*this, _Val, std::divides<>());
			return *this;
		}

		template <typename _T>
		_pixel_t<_T>& _pixel_t<_T>::operator *= (double const _Val)
		{
			const unsigned int chnls = channels(_fmt);
			const unsigned int chnlbits = bpp(_fmt) / chnls;
			const unsigned int mask = ~(~1 << (chnlbits - 1));

			unsigned int result = 0;
			for (std::size_t i = 0; i < chnls; ++i)
			{
				const unsigned int val = asref(_val) >> (i * chnlbits); 
				result |= ((unsigned int)(val * _Val) & mask) << (i * chnlbits); 
			}
			
			asref(_val) = result;
			return *this;
		}

		template <typename _T>
		_pixel_t<_T>& _pixel_t<_T>::operator /= (double const _Val)
		{
			const unsigned int chnls = channels(_fmt);
			const unsigned int chnlbits = bpp(_fmt) / chnls;
			const unsigned int mask = ~(~1 << (chnlbits - 1));

			unsigned int result = 0;
			for (std::size_t i = 0; i < chnls; ++i)
			{
				const unsigned int val = asref(_val) >> (i*chnlbits);
				result |= ((unsigned int)(val / _Val) & mask) << (i * chnlbits);
			}

			asref(_val) = result;
			return *this;
		}

		template <typename _T>
		_pixel_t<_T>::operator unsigned int() const noexcept
		{
			return asref(this->_val);
		}

		template <typename _T>
		bool operator == (_pixel_t<_T> const& _Left, _pixel_t<_T> const& _Right) noexcept
		{
			return asref(_Left._val) == asref(_Right._val)
				&& _Left._fmt == _Right._fmt;
		}

		template <typename _T>
		bool operator != (_pixel_t<_T> const& _Left, _pixel_t<_T> const& _Right) noexcept
		{
			return !(_Left == _Right);
		}

		template <typename _T>
		bool operator <= (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept
		{
			return false;
		}

		template <typename _T>
		bool operator >= (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept
		{
			return false;
		}

		template <typename _T>
		bool operator < (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept
		{
			return false;
		}

		template <typename _T>
		bool operator > (_pixel_t<_T> const&, _pixel_t<_T> const&) noexcept
		{
			return false;
		}

		template <typename _T>
		_pixel_t<_T> operator + (_pixel_t<_T> const& _Left, _pixel_t<_T> const& _Right) noexcept
		{
			return pixel_t(_Left) += _Right;
		}

		template <typename _T>
		_pixel_t<_T> operator - (_pixel_t<_T> const& _Left, _pixel_t<_T> const& _Right) noexcept
		{
			return pixel_t(_Left) -= _Right;
		}

		template <typename _T>
		_pixel_t<_T> operator * (_pixel_t<_T> const& _Left, _pixel_t<_T> const& _Right) noexcept
		{
			return pixel_t(_Left) *= _Right;
		}

		template <typename _T>
		_pixel_t<_T> operator / (_pixel_t<_T> const& _Left, _pixel_t<_T> const& _Right) noexcept
		{
			return pixel_t(_Left) /= _Right;
		}

		template <typename _T>
		_pixel_t<_T> operator * (_pixel_t<_T> const& _Left, double const _Right) noexcept
		{
			return pixel_t(_Left) *= _Right;
		}

		template <typename _T>
		_pixel_t<_T> operator * (double const _Left, _pixel_t<_T> const& _Right) noexcept
		{
			return pixel_t(_Right) *= _Left;
		}

		template <typename _T>
		_pixel_t<_T> operator / (_pixel_t<_T> const& _Left, double const _Right) noexcept
		{
			return pixel_t(_Left) /= _Right;
		}


		//// explicit instantiations

		template _pixel_t<unsigned int>::_pixel_t(_pixel_t<unsigned int&> const&);
		template _pixel_t<unsigned int&>::_pixel_t(_pixel_t<unsigned int*> const&);
		
		template _pixel_t<unsigned int>& _pixel_t<unsigned int>::operator = (_pixel_t<unsigned int&> const&);
		template _pixel_t<unsigned int&>& _pixel_t<unsigned int&>::operator = (_pixel_t<unsigned int> const&);
		template _pixel_t<unsigned int*>& _pixel_t<unsigned int*>::operator = (_pixel_t<unsigned int&> const&);

		template struct _pixel_t<unsigned int>; // value_type
		template struct _pixel_t<unsigned int&>; // reference
		template struct _pixel_t<unsigned int*>; // pointer

		template bool operator == <unsigned int>(_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template bool operator != <unsigned int>(_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template bool operator <= <unsigned int>(_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template bool operator >= <unsigned int>(_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template bool operator < <unsigned int>(_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template bool operator > <unsigned int>(_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;

		template _pixel_t<unsigned int> operator + (_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template _pixel_t<unsigned int> operator - (_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template _pixel_t<unsigned int> operator * (_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template _pixel_t<unsigned int> operator / (_pixel_t<unsigned int> const&, _pixel_t<unsigned int> const&) noexcept;
		template _pixel_t<unsigned int> operator * (_pixel_t<unsigned int> const&, double const) noexcept;
		template _pixel_t<unsigned int> operator * (double const, _pixel_t<unsigned int> const&) noexcept;
		template _pixel_t<unsigned int> operator / (_pixel_t<unsigned int> const&, double const) noexcept;
	}
}