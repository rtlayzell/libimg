#include <stdafx.h>
#include <cassert>

#include "bitmap_iterator.hpp"
#include "bitmap.hpp"


namespace libimg
{
	namespace detail
	{

		_BitmapIterator::_BitmapIterator(unsigned char* _DataPtr, pixel_format _Fmt)
			: _dataPtr(_DataPtr), _pixelFmt(_Fmt)
		{
		}

		_BitmapIterator::reference _BitmapIterator::operator [](std::size_t _Offset) noexcept
		{
			return reference(reinterpret_cast<unsigned int&>(
				*(this->_dataPtr + _Offset * bpp(this->_pixelFmt) / 8)), this->_pixelFmt);
		}

		_BitmapIterator::const_reference _BitmapIterator::operator [](std::size_t _Offset) const noexcept
		{
			return const_reference(reinterpret_cast<unsigned int&>(
				*(this->_dataPtr + _Offset * bpp(this->_pixelFmt) / 8)), this->_pixelFmt);
		}

		_BitmapIterator::reference _BitmapIterator::operator *() noexcept
		{
			return this->operator[](0);
		}

		_BitmapIterator::const_reference _BitmapIterator::operator *() const noexcept
		{
			return this->operator[](0);
		}

		_BitmapIterator::_Myt& _BitmapIterator::operator ++ (int) noexcept
		{
			return *this += 1;
		}

		_BitmapIterator::_Myt& _BitmapIterator::operator -- (int) noexcept
		{
			return *this -= 1;
		}

		_BitmapIterator::_Myt _BitmapIterator::operator ++ () noexcept
		{
			auto _tmp = *this; *this += 1;
			return _tmp;
		}

		_BitmapIterator::_Myt _BitmapIterator::operator -- () noexcept
		{
			auto _tmp = *this; *this -= 1;
			return _tmp;
		}

		_BitmapIterator::_Myt& _BitmapIterator::operator += (difference_type _Offs) noexcept
		{
			this->_dataPtr += _Offs * bpp(this->_pixelFmt) / 8;
			return *this;
		}

		_BitmapIterator::_Myt& _BitmapIterator::operator -= (difference_type _Offs) noexcept
		{
			this->_dataPtr -= _Offs * bpp(this->_pixelFmt) / 8;
			return *this;
		}

		bool operator == (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept
		{
			return _Left._dataPtr == _Right._dataPtr
				&& _Left._pixelFmt == _Right._pixelFmt;
		}

		bool operator != (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept
		{
			return !(_Left == _Right);
		}

		bool operator <= (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept
		{
			return (_Right < _Left);
		}

		bool operator >= (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept
		{
			return !(_Left < _Right);
		}

		bool operator < (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept
		{
			return _Left._dataPtr < _Right._dataPtr;
		}

		bool operator >(_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept
		{
			return !(_Left <= _Right);
		}

		std::ptrdiff_t operator - (_BitmapIterator const& _Left, _BitmapIterator const& _Right) noexcept
		{
			unsigned int bits = bpp(_Left._pixelFmt);

			assert(bits == bpp(_Right._pixelFmt));
			return (_Left._dataPtr - _Right._dataPtr) / (bits / 8); // byte-distance / bytes-per-pixel
		}

		_BitmapIterator operator + (_BitmapIterator const& _Left, std::ptrdiff_t _Right) noexcept
		{
			return _BitmapIterator(_Left) += _Right;
		}

		_BitmapIterator operator - (_BitmapIterator const& _Left, std::ptrdiff_t _Right) noexcept
		{
			return _BitmapIterator(_Left) -= _Right;
		}

		_BitmapIterator operator + (std::ptrdiff_t _Left, _BitmapIterator const& _Right) noexcept
		{
			return _BitmapIterator(_Right) += _Left;
		}
	}
}