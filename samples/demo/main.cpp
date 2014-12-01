#include <iostream>
#include <iomanip>
#include <fstream>
#include <iomanip>

#include <bitmap.hpp>
namespace ns = libimg;
//unsigned int bpp(pixel_format _Fmt)
//{
//	unsigned int value = (unsigned int)_Fmt;
//	return (value & 0xff);
//}
//
//pixel_format format(pixel_format _Fmt)
//{
//	return (pixel_format)((unsigned int)_Fmt & 0xff00);
//}
//
//unsigned short channels(pixel_format _Fmt)
//{
//	return ((unsigned int)_Fmt >> 16) & 0xf;
//}


typedef ns::detail::_pixel_t<unsigned int&> pixel_reference_t;


int main(int argc, char** argv)
{
	unsigned int c1 = 0x322432;
	unsigned int c2 = 0x00430f;

	ns::pixel_t p1(c1, ns::pixel_format::bpp24 | ns::pixel_format::rgb);
	ns::pixel_t p2(c2, ns::pixel_format::bpp24 | ns::pixel_format::rgb);


	std::cout << std::hex << std::showbase << (p1 + p1) << std::endl;
	std::cout << std::hex << std::showbase << (p1 * 2.0) << std::endl;
	std::cout << std::hex << std::showbase << (2.0 * p1) << std::endl;
	std::cout << std::hex << std::showbase << (p1 / 2.0) << std::endl;
	//std::cout << std::hex << std::showbase << (p1 - (p1 / 2.0)) << std::endl;
	std::cout << std::hex << std::showbase << ((p1 - p2) + p2) << std::endl;


	//std::cout << std::hex << std::showbase << << std::endl;

	return 0;
}