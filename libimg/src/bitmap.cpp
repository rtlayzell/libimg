#include <stdafx.h>

#include "bitmap.hpp"
#include "bitmap_iterator.hpp"

#include "impl\bitmap_impl.hpp"
#include "impl\bmp_bitmap_impl.hpp"
#include "impl\png_bitmap_impl.hpp"
#include "impl\jpg_bitmap_impl.hpp"
#include "impl\tif_bitmap_impl.hpp"


namespace libimg
{
	std::shared_ptr<bitmap::bitmap_impl> bitmap::_CreateBitmapImpl(std::string _Filename)
	{
		auto pimpl = std::make_shared<bitmap::bitmap_impl::png>();
		
		std::ifstream ifs(_Filename, std::ios::binary);
		if (!ifs) throw std::runtime_error("failed to open bitmap file.\n");

		pimpl->load(ifs);
		ifs.close();

		return pimpl;
	}

	bitmap::bitmap(bitmap const&) : _pimpl()
	{
	}

	bitmap::bitmap(std::istream& _Is) : _pimpl()
	{
		if (!_Is) throw std::runtime_error("invalid istream object.\n");
		this->_pimpl = std::make_shared<bitmap::bitmap_impl::png>();
		this->_pimpl->load(_Is);
	}

	bitmap::bitmap(std::string _Filename)
		: _pimpl(bitmap::_CreateBitmapImpl(_Filename))
	{
	}
	
	bitmap::bitmap(std::size_t _Width, std::size_t _Height, bitmap_format _Fmt)
		: _pimpl(std::make_shared<bitmap_impl::png>()) // defaults to .bmp file.
	{
	}

	bitmap::bitmap(std::size_t _Width, std::size_t _Height, bitmap::value_type _Color, bitmap_format _Fmt)
		: _pimpl(std::make_shared<bitmap_impl::png>()) // defaults to .bmp file.
	{
		this->clear(_Color);
	}

	bitmap::reference bitmap::operator()(std::size_t _X, std::size_t _Y) noexcept
	{
		return *(this->pixels() + _Y * this->width() + _X);
	}

	bitmap::const_reference bitmap::operator()(std::size_t _X, std::size_t _Y) const noexcept
	{
		return *(this->pixels() + _Y * this->width() + _X);
	}

	bitmap::iterator bitmap::pixels() noexcept
	{
		return bitmap::iterator(
			this->_pimpl ? this->_pimpl->data() : nullptr,
			this->format());
	}

	bitmap::const_iterator bitmap::pixels() const noexcept
	{
		return bitmap::const_iterator(
			this->_pimpl ? this->_pimpl->data() : nullptr,
			this->format());
	}

	bitmap::pointer bitmap::raw() noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->data();
		return nullptr;
	}

	bitmap::const_pointer bitmap::raw() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->data();
		return nullptr;
	}

	bool bitmap::empty() const noexcept
	{
		return (this->begin() == this->end());
	}

	bitmap_format bitmap::format() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->format();
		return bitmap_format::unknown;
	}
	
	bitmap::size_type bitmap::size() const noexcept
	{
		return 0;
	}

	bitmap_bitdepth bitmap::bit_depth() const noexcept
	{
		return bitmap_bitdepth::bit32;
	}

	bitmap::size_type bitmap::dpi() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->dpi();
		return 0;
	}
	
	bitmap::size_type bitmap::width() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->width();
		return 0;
	}
	
	bitmap::size_type bitmap::height() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->height();
		return 0;
	}

	void bitmap::clear(unsigned int _Color) noexcept
	{
		std::fill(this->begin(), this->end(), _Color);
	}

	void bitmap::assign(bitmap&& _Other)
	{
		if (this == &_Other) return;
		this->_pimpl = _Other._pimpl;
		_Other._pimpl = nullptr;
	}
	
	void bitmap::assign(bitmap const& _Other)
	{
		// could use copy-on-write semantics?
		bitmap b(_Other);
		std::copy(_Other.begin(), _Other.end(), this->begin());

		this->assign(std::move(b));
	}

	bitmap& bitmap::operator = (bitmap const& _Other)
	{
		this->assign(_Other);
		return *this;
	}

	bitmap::iterator bitmap::begin() noexcept
	{
		return this->pixels();
	}
	
	bitmap::iterator bitmap::end() noexcept
	{
		return this->pixels() + (this->width() * this->height());
	}
	
	bitmap::const_iterator bitmap::begin() const noexcept
	{
		return this->pixels();
	}
	
	bitmap::const_iterator bitmap::end() const noexcept
	{
		return this->pixels() + (this->width() * this->height());
	}
	
	bitmap::const_iterator bitmap::cbegin() const noexcept
	{
		return this->pixels();
	}
	
	bitmap::const_iterator bitmap::cend() const noexcept
	{
		return this->pixels() + (this->width() * this->height());
	}

	bitmap::reverse_iterator bitmap::rbegin() noexcept
	{
		return bitmap::reverse_iterator(this->end());
	}
	
	bitmap::reverse_iterator bitmap::rend() noexcept
	{
		return bitmap::reverse_iterator(this->begin());
	}
	
	bitmap::const_reverse_iterator bitmap::rbegin() const noexcept
	{
		return bitmap::const_reverse_iterator(this->end());
	}
	
	bitmap::const_reverse_iterator bitmap::rend() const noexcept
	{
		return bitmap::const_reverse_iterator(this->begin());
	}
	
	bitmap::const_reverse_iterator bitmap::crbegin() const noexcept
	{
		return bitmap::const_reverse_iterator(this->end());
	}
	
	bitmap::const_reverse_iterator bitmap::crend() const noexcept
	{
		return bitmap::const_reverse_iterator(this->begin());
	}

	void bitmap::swap(bitmap& _Other)
	{
		this->_pimpl.swap(_Other._pimpl);
	}

	void bitmap::read(std::string _Filename)
	{
		std::ifstream ifs(_Filename, std::ios::in | std::ios::binary);
		if (!ifs) throw 10;

		this->read(ifs);
		ifs.close();
	}

	void bitmap::write(std::string _Filename)
	{
		std::ofstream ofs(_Filename, std::ios::out | std::ios::binary);
		if (!ofs) throw 20;
		
		this->write(ofs);
		ofs.flush();
		ofs.close();
	}

	void bitmap::read(std::istream& _Is)
	{
		_Is >> *this;
	}

	void bitmap::write(std::ostream& _Os)
	{
		_Os << *this;
	}

	bool operator == (bitmap const& _Left, bitmap const& _Right)
	{
		return _Left.format() == _Right.format()
			&& _Left.dpi() == _Right.dpi()
			&& _Left.width() == _Right.width()
			&& _Left.height() == _Right.height()
			&& std::equal(_Left.begin(), _Left.end(), _Right.begin(), _Right.end());
	}

	bool operator != (bitmap const& _Left, bitmap const& _Right) 
	{
		return !(_Left == _Right);
	}

	bool operator == (bitmap const& _Left, ::std::nullptr_t const)
	{
		return _Left.empty();
	}

	bool operator != (bitmap const& _Left, ::std::nullptr_t const _Right)
	{
		return !(_Left == _Right);
	}

	bool operator == (::std::nullptr_t const _Left, bitmap const& _Right)
	{
		return (_Right == _Left);
	}

	bool operator != (::std::nullptr_t const _Left, bitmap const& _Right)
	{
		return !(_Right == _Left);
	}

}