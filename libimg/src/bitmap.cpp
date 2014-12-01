#include <stdafx.h>
#include <algorithm>
#include <locale>

#include "bitmap.hpp"
#include "bitmap_iterator.hpp"

#include "impl\bitmap_impl.hpp"
#include "impl\bmp_bitmap_impl.hpp"
#include "impl\png_bitmap_impl.hpp"
#include "impl\jpg_bitmap_impl.hpp"
#include "impl\tif_bitmap_impl.hpp"

static std::string _GetPathExtension(std::string _Filename);

namespace libimg
{
	bitmap::bitmap(bitmap const&) : _pimpl()
	{
		this->_pimpl = std::make_shared<bitmap::bitmap_impl::bmp>();
	}

	bitmap::bitmap(std::istream& _Is) : _pimpl()
	{
		if (!_Is) throw std::runtime_error("invalid istream object.\n");
		this->_pimpl = std::make_shared<bitmap::bitmap_impl::bmp>();
		this->_pimpl->load(_Is);
	}

	bitmap::bitmap(std::string _Filename)
		: _pimpl(bitmap::_CreateBitmapImpl(_Filename))
	{
	}

	bitmap::bitmap(std::size_t _Width, std::size_t _Height, pixel_format _Fmt)
		: bitmap::bitmap(_Width, _Height, 0xFF000000, _Fmt)
	{

	}

	bitmap::bitmap(std::size_t _Width, std::size_t _Height, bitmap::value_type _Color, pixel_format _Fmt)
		: _pimpl(std::make_shared<bitmap_impl::bmp>()) // defaults to .bmp file.
	{
		this->clear(_Color);
	}

	bitmap::iterator bitmap::operator [](std::size_t _Row) noexcept
	{
		return (this->pixels() + _Row * this->width());
	}

	bitmap::const_iterator bitmap::operator [](std::size_t _Row) const noexcept
	{
		return (this->pixels() + _Row * this->width());
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

	bitmap::size_type bitmap::size() const noexcept
	{
		return 0;
	}

	unsigned short bitmap::depth() const noexcept
	{
		return libimg::bpp(this->format());
	}

	unsigned short bitmap::channels() const noexcept
	{
		return libimg::channels(this->format());
	}

	pixel_format bitmap::format() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->format();
		return pixel_format::undefined;
	}

	bitmap::size_type bitmap::dpi() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->xdpi();
		return 0;
	}

	bitmap::size_type bitmap::xdpi() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->xdpi();
		return 0;
	}

	bitmap::size_type bitmap::ydpi() const noexcept
	{
		if (this->_pimpl)
			return this->_pimpl->ydpi();
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

	void bitmap::resize(std::size_t _Width, std::size_t _Height)
	{

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

	std::shared_ptr<bitmap::bitmap_impl> bitmap::_CreateBitmapImpl(std::string _Filename)
	{
		auto ext = _GetPathExtension(_Filename);
		std::shared_ptr<bitmap::bitmap_impl> pimpl = nullptr;

		std::transform(std::begin(ext), std::end(ext), std::begin(ext), tolower);
		if (ext == ".bmp") pimpl = std::make_shared<bitmap::bitmap_impl::bmp>();
		else if (ext == ".png") pimpl = std::make_shared<bitmap::bitmap_impl::png>();
		//else if (ext == ".jpg" || ext == ".jpeg") pimpl = std::make_shared<bitmap::bitmap_impl::jpg>();
		//else if (ext == ".tif" || ext == ".tiff") pimpl = std::make_shared<bitmap::bitmap_impl::tif>();
		else return nullptr;

		std::ifstream ifs(_Filename, std::ios::binary);
		if (!ifs) throw std::runtime_error("failed to open bitmap file.\n");

		pimpl->load(ifs);
		ifs.close();

		return pimpl;
	}
}

std::string _GetPathExtension(std::string _Filename)
{
	auto first = std::rbegin(_Filename);
	auto last = std::rend(_Filename);
	auto curr = first;

	for (; curr != last; ++curr)
	{
		if (*curr == '.')
			return std::string((curr + 1).base(), first.base());

		// _Filename does not specify an extension.
		if (*curr == '\\' || *curr == ':')
			break;
	}

	return std::string();
}