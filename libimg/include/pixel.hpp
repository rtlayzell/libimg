#ifndef HEADER_PIXEL_HPP
#define HEADER_PIXEL_HPP

#include <ostream>
#include <istream>

#include "pixel_format.hpp"

namespace libimg
{
	namespace detail
	{ struct _Pixel; }

	typedef detail::_Pixel pixel_t;
	
	namespace detail
	{

		struct _Pixel
		{
		public:
			// used to detect if this pixel has merely been casted from a bitmap 
			// iterator. In which case the _value needs to be casted. Always use the 
			// _Get/_SetValue pair when dealing the value of a _Pixel.
			unsigned char _unused = 0;

			unsigned int _value;
			pixel_format _fmt;

			unsigned int _GetValue() const noexcept;
			void _SetValue(unsigned int _Val) noexcept;

			template <typename _Fn2> void _Transform(_Pixel&, _Pixel const&, _Fn2) noexcept;
			template <typename _Fn2> void _TransformScalar(_Pixel&, unsigned int, _Fn2) noexcept;

			friend bool operator == (_Pixel const&, _Pixel const&) noexcept;
			friend bool operator  < (_Pixel const&, _Pixel const&) noexcept;

			template <typename _Elem, typename _Traits>
			friend std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>&, _Pixel const&) noexcept;

		public:
			typedef unsigned int value_type;
			typedef unsigned int pointer;
			typedef unsigned int reference;

			_Pixel() = default;
			_Pixel(_Pixel&&) noexcept;
			_Pixel(_Pixel const&) noexcept;

			_Pixel(unsigned int _Val, pixel_format _Fmt) noexcept;

			_Pixel& operator = (_Pixel&&) noexcept;
			_Pixel& operator = (_Pixel const&) noexcept;
			_Pixel& operator = (const unsigned int) noexcept;

			_Pixel& operator += (_Pixel const&) noexcept;
			_Pixel& operator -= (_Pixel const&) noexcept;
			_Pixel& operator *= (_Pixel const&) noexcept;
			_Pixel& operator /= (_Pixel const&) noexcept;

			_Pixel& operator &= (_Pixel const&) noexcept;
			_Pixel& operator |= (_Pixel const&) noexcept;
			_Pixel& operator ^= (_Pixel const&) noexcept;

			_Pixel operator ~() const noexcept;

			pixel_format format() const noexcept;

			operator unsigned int() const noexcept;
		};

		_Pixel operator + (_Pixel const&, _Pixel const&) noexcept;
		_Pixel operator - (_Pixel const&, _Pixel const&) noexcept;
		_Pixel operator * (_Pixel const&, _Pixel const&) noexcept;
		_Pixel operator / (_Pixel const&, _Pixel const&) noexcept;

		_Pixel operator & (_Pixel const&, _Pixel const&) noexcept;
		_Pixel operator | (_Pixel const&, _Pixel const&) noexcept;
		_Pixel operator ^ (_Pixel const&, _Pixel const&) noexcept;

		bool operator == (_Pixel const&, _Pixel const&) noexcept;
		bool operator <= (_Pixel const&, _Pixel const&) noexcept;
		bool operator >= (_Pixel const&, _Pixel const&) noexcept;
		bool operator  < (_Pixel const&, _Pixel const&) noexcept;
		bool operator  > (_Pixel const&, _Pixel const&) noexcept;

		template <typename _Elem, typename _Traits>
		std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>& _Os, _Pixel const& _Val) noexcept
		{
			std::ios init(NULL);
			init.copyfmt(_Os);

			_Os << std::hex;
			if (_Os.flags() & std::ios::showbase)
				_Os << std::noshowbase << '#';

			_Os << std::setfill('0')
				<< std::setw(bpp(_Val._fmt) / 4)
				<< (_Val._GetValue());

			_Os.copyfmt(init);
			return _Os;
		}
	}
}

#endif