#ifndef HEADER_NS_BITMAP_HPP
#define HEADER_NS_BITMAP_HPP

#pragma once

#ifdef _WIN32
#define _SCL_SECURE_NO_WARNINGS
#endif

#include <ostream>
#include <istream>
#include <fstream>
#include <string>
#include <memory>

#include "bitmap_io.hpp"
#include "bitmap_iterator.hpp"
#include "pixel_format.hpp"


namespace libimg
{
	class bitmap
	{
	private:
		struct bitmap_impl;
		std::shared_ptr<bitmap_impl> _pimpl;

		static std::shared_ptr<bitmap::bitmap_impl> _CreateBitmapImpl(std::string _Path);
		static std::shared_ptr<bitmap::bitmap_impl> _CreateClone(); /* stub */

	public:
		typedef unsigned int value_type;
		typedef unsigned char* pointer;
		typedef value_type& reference;
		typedef unsigned char const* const_pointer;
		typedef value_type const& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef detail::_BitmapIterator iterator;
		typedef detail::_BitmapIterator const_iterator;
		typedef std::reverse_iterator<detail::_BitmapIterator> reverse_iterator;
		typedef std::reverse_iterator<detail::_BitmapIterator> const_reverse_iterator;
		
		/* \brief Constructs an empty bitmap. */
		bitmap() = default;
		bitmap(bitmap&&) = default;
		bitmap(bitmap const&);

		explicit bitmap(std::string);
		explicit bitmap(std::istream&);
		bitmap(std::size_t _Width, std::size_t _Height, pixel_format);
		bitmap(std::size_t _Width, std::size_t _Height, value_type _Color, pixel_format);

		/* \brief Returns an iterator to the start of the row specified. */
		iterator operator [](std::size_t) noexcept;

		/* \brief Returns a const iterator to the start of the row specified. */
		const_iterator operator [](std::size_t _Row) const noexcept;

		bitmap& operator = (bitmap&&) = default;
		bitmap& operator = (bitmap const&);

		iterator pixels() noexcept;
		const_iterator pixels() const noexcept;

		pointer raw() noexcept;
		const_pointer raw() const noexcept;

		/* \brief Returns a value indicating whether the bitmap is empty/uninitialized, or not. */
		bool empty() const noexcept;

		/* \brief Returns the size of the image in bytes. */
		size_type size() const noexcept;
		
		/* \brief Returns the bits-per-pixel of the image pixels */
		unsigned short depth() const noexcept;

		/* \brief Returns the number of channels that define a pixel in the bitmap. */
		unsigned short channels() const noexcept;

		/* \brief Returns the enumerated value of the bitmap format. */
		pixel_format format() const noexcept;

		/* \brief Returns the resolution of the image, or 0 if xdpi != ydpi. */
		size_type dpi() const noexcept;

		/* \brief Returns the horizontal resolution of the bitmap image. */
		size_type xdpi() const noexcept;

		/* \brief Returns the vertical resolution of the bitmap image. */
		size_type ydpi() const noexcept;

		/* \brief Returns the width of the bitmap in pixels. */
		size_type width() const noexcept;
		
		/* \brief Returns the height of the bitmap in pixels. */
		size_type height() const noexcept;

		/* \brief Clears the bitmap using the specified color. */
		void clear(unsigned int = 0xFF << 24) noexcept;

		/* \brief Resizes the bitmap to the specified width and height. */
		void resize(std::size_t _Width, std::size_t _Height);

		/* \brief Assigns the contents of this bitmap to the contents of the one specified. */
		void assign(bitmap&&);

		/* \brief Assigns the contents of this bitmap to the contents of the one specified. */
		void assign(bitmap const&);

		/* \brief Returns an iterator to the beginning of the bitmap pixels. */
		iterator begin() noexcept;

		/* \brief Returns an iterator to the end of the bitmap pixels. */
		iterator end() noexcept;

		/* \brief Returns a const iterator to the end of the bitmap pixels. */
		const_iterator begin() const noexcept;

		/* \brief Returns a const iterator to the end of the bitmap pixels. */
		const_iterator end() const noexcept;

		/* \brief Returns a const iterator to the beginning of the bitmap pixels. */
		const_iterator cbegin() const noexcept;

		/* \brief Returns a const iterator to the end of the bitmap pixels. */
		const_iterator cend() const noexcept;

		/* \brief Returns a reverse iterator to the beginning of the bitmap pixels. */
		reverse_iterator rbegin() noexcept;

		/* \brief Returns a reverse iterator to the end of the bitmap pixels. */
		reverse_iterator rend() noexcept;

		/* \brief Returns a const reverse iterator to the beginning of the bitmap pixels. */
		const_reverse_iterator rbegin() const noexcept;

		/* \brief Returns a const reverse iterator to the end of the bitmap pixels. */
		const_reverse_iterator rend() const noexcept;

		/* \brief Returns a const reverse iterator to the beginning of the bitmap pixels. */
		const_reverse_iterator crbegin() const noexcept;

		/* \brief Returns a const reverse iterator to the end of the bitmap pixels. */
		const_reverse_iterator crend() const noexcept;

		/* \brief Swaps the bitmap contents with the contents of the bitmap specified. */
		void swap(bitmap&);

		/* \brief Reads bitmap data from the file specified. */
		void read(std::string);

		/* \brief Writes bitmap data to the file specified. */
		void write(std::string);

		/* \brief Reads the bitmap data from the input stream specified. */
		void read(std::istream&);

		/* \brief Writes the bitmap data to the output stream specified. */
		void write(std::ostream&);

		/* \brief Writes the bitmap data to the output stream specified. */
		friend std::ostream& operator << (std::ostream&, bitmap const&);
		
		/* \brief Reads the bitmap data from the input stream specified. */
		friend std::istream& operator >> (std::istream&, bitmap&);
	};

	/* \brief Determines if one bitmap is equal to another, true if they are equal, false otherwise. */
	bool operator == (bitmap const&, bitmap const&);

	/* \brief Determines if one bitmap is not equal to another, true if they are not equal, false otherwise. */
	bool operator != (bitmap const&, bitmap const&);

	/* \brief Determines if one bitmap is uninitialized, true if it is uninitialized, false otherwise. */
	bool operator == (bitmap const&, ::std::nullptr_t const);

	/* \brief Determines if one bitmap is not uninitialized, true if it is not uninitialized, false otherwise. */
	bool operator != (bitmap const&, ::std::nullptr_t const);

	/* \brief Determines if one bitmap is uninitialized, true if it is uninitialized, false otherwise. */
	bool operator == (::std::nullptr_t const, bitmap const&);

	/* \brief Determines if one bitmap is not uninitialized, true if it is not uninitialized, false otherwise. */
	bool operator != (::std::nullptr_t const, bitmap const&);

	/* \brief Writes the bitmap data to the output stream specified. */
	std::ostream& operator << (std::ostream& _Os, bitmap const&);
	
	/* \brief Reads the bitmap data from the input stream specified. */
	std::istream& operator >> (std::istream& _Is, bitmap&);
}

#endif /*HEADER_NS_BITMAP_HPP*/
