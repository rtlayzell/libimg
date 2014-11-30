#include <stdafx.h>
#include <cassert>

#include "bitmap_iterator.hpp"

#define FMT_BPP(X) (((std::size_t)X) & 0xFF)

namespace libimg
{
	_pixel_t::_pixel_t(unsigned int& _Val, pixel_format& _Fmt)
		: _val(_Val), _fmt(_Fmt) {}

	_pixel_t& _pixel_t::operator = (unsigned int _Val)
	{
		if (!(((char)this->_fmt) & ((char)_Val)))
			throw std::runtime_error("invalid pixel_t format during assignment.");

		return *this;
	}

	_pixel_t& _pixel_t::operator = (_pixel_t const& _Other)
	{
		if (!(((char)this->_fmt) & ((char)_Other._fmt)))
			throw std::runtime_error("invalid pixel_t format during assignment.");

		return *this = _Other._val;
	}

	_pixel_t& _pixel_t::operator += (unsigned int _Val)
	{
		this->_val = this->_val + _Val;
		return *this;
	}

	_pixel_t& _pixel_t::operator -= (unsigned int _Val)
	{

		return *this;
	}

	_pixel_t& _pixel_t::operator *= (unsigned int _Val)
	{

		return *this;
	}

	_pixel_t& _pixel_t::operator /= (unsigned int _Val)
	{

		return *this;
	}


	bool operator == (_pixel_t const& _Left, _pixel_t const& _Right)
	{
		return _Left._val == _Right._val
			&& _Left._fmt == _Right._fmt;
	}

	bool operator != (_pixel_t const& _Left, _pixel_t const& _Right)
	{
		return !(_Left == _Right);
	}
}