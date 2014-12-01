#include <stdafx.h>
#include <cassert>

#include "bitmap_iterator.hpp"
#include "bitmap.hpp"


namespace libimg
{
	_bitmap_iterator::_bitmap_iterator(unsigned char* _DataPtr, pixel_format _Fmt)
		: _dataPtr(_DataPtr), _bmpFmt(_Fmt)
	{
	}

	_bitmap_iterator::reference _bitmap_iterator::operator [](std::size_t _Offset) noexcept
	{
		return reference(reinterpret_cast<unsigned int&>(
			*(this->_dataPtr + _Offset * bpp(this->_bmpFmt) / 8)), this->_bmpFmt);
	}

	_bitmap_iterator::const_reference const _bitmap_iterator::operator [](std::size_t _Offset) const noexcept
	{
		return const_reference(reinterpret_cast<unsigned int&>(
			*(this->_dataPtr + _Offset * bpp(this->_bmpFmt) / 8)), this->_bmpFmt);
	}

	_bitmap_iterator::reference _bitmap_iterator::operator *() noexcept
	{
		return this->operator[](0);
	}

	_bitmap_iterator::const_reference const _bitmap_iterator::operator *() const noexcept
	{
		return this->operator[](0);
	}

	_bitmap_iterator::_Myt& _bitmap_iterator::operator ++ (int) noexcept
	{
		return *this += 1;
	}

	_bitmap_iterator::_Myt& _bitmap_iterator::operator -- (int) noexcept
	{
		return *this -= 1;
	}

	_bitmap_iterator::_Myt _bitmap_iterator::operator ++ () noexcept
	{
		auto _tmp = *this; *this += 1;
		return _tmp;
	}

	_bitmap_iterator::_Myt _bitmap_iterator::operator -- () noexcept
	{
		auto _tmp = *this; *this -= 1;
		return _tmp;
	}

	_bitmap_iterator::_Myt& _bitmap_iterator::operator += (difference_type _Offs) noexcept
	{
		
		this->_dataPtr += _Offs * bpp(this->_bmpFmt) / 8;
		return *this;
	}

	_bitmap_iterator::_Myt& _bitmap_iterator::operator -= (difference_type _Offs) noexcept
	{
		this->_dataPtr -= _Offs * bpp(this->_bmpFmt) / 8;
		return *this;
	}

	bool operator == (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right)
	{
		return _Left._dataPtr == _Right._dataPtr
			&& _Left._bmpFmt == _Right._bmpFmt;
	}

	bool operator != (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right)
	{
		return !(_Left == _Right);
	}

	bool operator <= (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right)
	{
		return (_Right < _Left);
	}

	bool operator >= (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right)
	{
		return !(_Left < _Right);
	}

	bool operator < (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right)
	{
		return _Left._dataPtr < _Right._dataPtr;
	}

	bool operator > (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right)
	{
		return !(_Left <= _Right);
	}

	std::ptrdiff_t operator - (_bitmap_iterator const& _Left, _bitmap_iterator const& _Right)
	{
		assert(bpp(_Left._bmpFmt) == bpp(_Right._bmpFmt));
		return (_Left._dataPtr - _Right._dataPtr) / 8;
	}

	_bitmap_iterator operator + (_bitmap_iterator const& _Left, std::ptrdiff_t _Right)
	{
		return _bitmap_iterator(_Left) += _Right;
	}

	_bitmap_iterator operator - (_bitmap_iterator const& _Left, std::ptrdiff_t _Right)
	{
		return _bitmap_iterator(_Left) -= _Right;
	}

	_bitmap_iterator operator + (std::ptrdiff_t _Left, _bitmap_iterator const& _Right)
	{
		return _bitmap_iterator(_Right) += _Left;
	}
}