#include "stdafx.h"
#include <iostream>
#include <type_traits>
#include <functional>
#include <cassert>
#include "pixel.hpp"

namespace libimg
{
	namespace detail
	{
		unsigned int _Pixel::_GetValue() const noexcept
		{
			if (_unused) // are we actually an iterator?
				return *reinterpret_cast<unsigned int*>(_value);
			return _value;
		}

		void _Pixel::_SetValue(unsigned int _Val) noexcept
		{
			if (_unused) // are we actually an iterator?
			{
				*reinterpret_cast<unsigned int*>(_value) = _Val;
				return;
			}

			_value = _Val;
		}

		_Pixel::_Pixel(_Pixel&& _Other) noexcept
			: _fmt(_Other._fmt)
		{
			*this = _Other;
		}

		_Pixel::_Pixel(_Pixel const& _Other) noexcept
			: _fmt(_Other._fmt)
		{
			*this = _Other;
		}

		_Pixel::_Pixel(unsigned int _Val, pixel_format _Fmt) noexcept
			: _value(_Val), _fmt(_Fmt)
		{
			// process the value with _TransformScalar method, this ensures that 
			// each channel and the number of bits per channel are adhere with overflow.
			_TransformScalar(*this, 0, [](unsigned int x, unsigned int y) { return x; });
		}

		_Pixel& _Pixel::operator = (_Pixel&& _Other) noexcept
		{
			assert(bpp(_fmt) == bpp(_Other._fmt));

			_SetValue(_Other._GetValue());
			return *this;
		}

		_Pixel& _Pixel::operator = (_Pixel const& _Other) noexcept
		{
			assert(bpp(_fmt) == bpp(_Other._fmt));

			_SetValue(_Other._GetValue());
			return *this;
		}

		_Pixel& _Pixel::operator = (const unsigned int _Val) noexcept
		{
			_SetValue(_Val);
			_TransformScalar(*this, 0, [](unsigned int x, unsigned int y) { return x; });

			return *this;
		}

		_Pixel& _Pixel::operator += (_Pixel const& _Other) noexcept
		{
			_Transform(*this, _Other, std::plus<>());
			return *this;
		}

		_Pixel& _Pixel::operator -= (_Pixel const& _Other) noexcept
		{
			_Transform(*this, _Other, std::minus<>());
			return *this;
		}

		_Pixel& _Pixel::operator *= (_Pixel const& _Other) noexcept
		{
			_Transform(*this, _Other, std::multiplies<>());
			return *this;
		}

		_Pixel& _Pixel::operator /= (_Pixel const& _Other) noexcept
		{
			_Transform(*this, _Other, std::divides<>());
			return *this;
		}

		_Pixel& _Pixel::operator &= (_Pixel const& _Other) noexcept
		{
			//_Transform(*this, _Other, std::plus<>());
			return *this;
		}

		_Pixel& _Pixel::operator |= (_Pixel const& _Other) noexcept
		{
			//_Transform(*this, _Other, std::plus<>());
			return *this;
		}

		_Pixel& _Pixel::operator ^= (_Pixel const& _Other) noexcept
		{
			//_Transform(*this, _Other, std::plus<>());
			return *this;
		}

		_Pixel _Pixel::operator ~() const noexcept
		{
			return { ~_GetValue(), _fmt };
		}

		pixel_format _Pixel::format() const noexcept
		{
			return this->_fmt;
		}

		_Pixel::operator unsigned int() const noexcept
		{
			return this->_GetValue();
		}

		_Pixel operator + (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Pixel(_Left) += _Right;
		}

		_Pixel operator - (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Pixel(_Left) -= _Right;
		}

		_Pixel operator * (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Pixel(_Left) *= _Right;
		}

		_Pixel operator / (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Pixel(_Left) /= _Right;
		}


		_Pixel operator & (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Pixel(_Left) &= _Right;
		}

		_Pixel operator | (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Pixel(_Left) |= _Right;
		}

		_Pixel operator ^ (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Pixel(_Left) ^= _Right;
		}


		bool operator == (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Left._GetValue() == _Right._GetValue()
				&& _Left._fmt == _Right._fmt;
		}

		bool operator <= (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return !(_Right < _Left);
		}

		bool operator >= (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return !(_Left < _Right);
		}

		bool operator < (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			// yeah, this isn't right...
			return _Left._GetValue() < _Right._GetValue();
		}

		bool operator > (_Pixel const& _Left, _Pixel const& _Right) noexcept
		{
			return _Right < _Left;
		}

		template <typename _Fn2>
		void _Pixel::_Transform(_Pixel& _Val1, _Pixel const& _Val2, _Fn2 _Func) noexcept
		{
			const unsigned int chnls = channels(_Val1._fmt);
			const unsigned int chnlbits = bpp(_Val1._fmt) / chnls;
			const unsigned int mask = ~(~1ULL << (chnlbits - 1));

			unsigned int result = 0;
			register unsigned int actualVal1 = _Val1._GetValue();
			register unsigned int actualVal2 = _Val2._GetValue();
			for (std::size_t i = 0; i < chnls; ++i)
			{
				const unsigned int val1 = (actualVal1 >> (i * chnlbits)) & mask; // shift into the lowest channel.
				const unsigned int val2 = (actualVal2 >> (i * chnlbits)) & mask; // shift into the lowest channel.
				result |= (_Func(val1, val2) & mask) << (i * chnlbits); // perform operation, mask bits from lowest channel, shift back into correct channel.
			}

			_Val1._SetValue(result);
		}

		template <typename _Fn2>
		void _Pixel::_TransformScalar(_Pixel& _Val1, unsigned int _Val2, _Fn2 _Func) noexcept
		{
			const unsigned int chnls = channels(_Val1._fmt);
			const unsigned int chnlbits = bpp(_Val1._fmt) / chnls;
			const unsigned int mask = ~(~1ULL << (chnlbits - 1));

			unsigned int result = 0;

			unsigned int actualVal = _Val1._GetValue();
			for (std::size_t i = 0; i < chnls; ++i)
			{
				const unsigned int val = (_Val1._GetValue() >> (i * chnlbits)) & mask;
				result |= ((unsigned int)(_Func(val, _Val2)) & mask) << (i * chnlbits);
			}

			_Val1._SetValue(result);
		}
	}
}