#include <stdafx.h>
#include <iostream>

#include "bitmap.hpp"
#include "bitmap_iterator.hpp"

#include "impl\bitmap_impl.hpp"
#include "impl\bmp_bitmap_impl.hpp"
#include "impl\png_bitmap_impl.hpp"
#include "impl\jpg_bitmap_impl.hpp"
#include "impl\tif_bitmap_impl.hpp"


namespace libimg
{
	std::ostream& bmp(std::ostream& _Os)
	{
		std::cout << "converting to bmp" << std::endl;
		return _Os;
	}

	std::ostream& png(std::ostream& _Os)
	{
		std::cout << "converting to png" << std::endl;
		return _Os;
	}

	std::ostream& jpg(std::ostream& _Os)
	{
		std::cout << "converting to jpg" << std::endl;
		return _Os;
	}

	std::ostream& tif(std::ostream& _Os)
	{
		std::cout << "converting to tif" << std::endl;
		return _Os;
	}

	std::ostream& operator << (std::ostream& _Os, bitmap const& _Bitmap)
	{
		auto pfunc = reinterpret_cast<std::ostream&(*)(std::ostream&)>(_Os.pword(0));
		if (pfunc == 0)
			_Bitmap._pimpl->save(_Os); // use own pimpl

		// perform the necessary conversion 
		// to the requested bitmap format.
		if (pfunc == &libimg::bmp) std::cout << "converting to bmp file format." << std::endl;
		if (pfunc == &libimg::png) std::cout << "converting to png file format." << std::endl;
		if (pfunc == &libimg::tif) std::cout << "converting to tif file format." << std::endl;
		if (pfunc == &libimg::jpg) std::cout << "converting to jpg file format." << std::endl;

		return _Os;
	}

	std::istream& operator >> (std::istream& _Is, bitmap& _Bitmap)
	{
		_Bitmap._pimpl->load(_Is);
		return _Is;
	}
}