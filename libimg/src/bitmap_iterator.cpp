#include <stdafx.h>

#include "bitmap_iterator.hpp"
#include "bitmap.hpp"

namespace libimg
{
	bitmap_iterator::bitmap_iterator(unsigned char* _DataPtr, bitmap_format _Fmt)
		: _dataPtr(_DataPtr), _bmpFmt(_Fmt)
	{
	}

	unsigned int& bitmap_iterator::operator [](std::size_t _Offset) noexcept
	{
		return reinterpret_cast<unsigned int&>(
			*(this->_dataPtr + _Offset * ((std::size_t)this->_bmpFmt / 8)));
	}

	unsigned int const& bitmap_iterator::operator [](std::size_t _Offset) const noexcept
	{
		return reinterpret_cast<unsigned int&>(
			*(this->_dataPtr + _Offset * ((std::size_t)this->_bmpFmt / 8)));
	}

	unsigned int& bitmap_iterator::operator *() noexcept
	{
		return this->operator[](0);
	}

	unsigned int const& bitmap_iterator::operator *() const noexcept
	{
		return this->operator[](0);
	}

	bitmap_iterator::_Myt& bitmap_iterator::operator ++ (int) noexcept
	{
		return *this += 1;
	}

	bitmap_iterator::_Myt& bitmap_iterator::operator -- (int) noexcept
	{
		return *this -= 1;
	}

	bitmap_iterator::_Myt& bitmap_iterator::operator ++ () noexcept
	{
		return *this += 1;
	}

	bitmap_iterator::_Myt& bitmap_iterator::operator -- () noexcept
	{
		return *this -= 1;
	}

	bitmap_iterator::_Myt& bitmap_iterator::operator += (difference_type _Offs) noexcept
	{
		this->_dataPtr += _Offs * ((std::size_t)this->_bmpFmt / 8);
		return *this;
	}

	bitmap_iterator::_Myt& bitmap_iterator::operator -= (difference_type _Offs) noexcept
	{
		this->_dataPtr += _Offs * ((std::size_t)this->_bmpFmt / 8);
		return *this;
	}

	bool operator == (bitmap_iterator const& _Left, bitmap_iterator const& _Right)
	{
		return _Left._dataPtr == _Right._dataPtr
			&& _Left._bmpFmt == _Right._bmpFmt;
	}

	bool operator != (bitmap_iterator const& _Left, bitmap_iterator const& _Right)
	{
		return !(_Left == _Right);
	}

	bool operator <= (bitmap_iterator const& _Left, bitmap_iterator const& _Right)
	{
		return (_Right < _Left);
	}

	bool operator >= (bitmap_iterator const& _Left, bitmap_iterator const& _Right)
	{
		return !(_Left < _Right);
	}

	bool operator < (bitmap_iterator const& _Left, bitmap_iterator const& _Right)
	{
		return _Left._dataPtr < _Right._dataPtr;
	}

	bool operator > (bitmap_iterator const& _Left, bitmap_iterator const& _Right)
	{
		return !(_Left <= _Right);
	}

	std::ptrdiff_t operator - (bitmap_iterator const& _Left, bitmap_iterator const& _Right)
	{
		return (_Left._dataPtr - _Right._dataPtr) / 8;
	}

	bitmap_iterator operator + (bitmap_iterator const& _Left, std::ptrdiff_t _Right)
	{
		return bitmap_iterator(_Left) += _Right;
	}

	bitmap_iterator operator - (bitmap_iterator const& _Left, std::ptrdiff_t _Right)
	{
		return bitmap_iterator(_Left) -= _Right;
	}

	bitmap_iterator operator + (std::ptrdiff_t _Left, bitmap_iterator const& _Right)
	{
		return bitmap_iterator(_Right) += _Left;
	}
}