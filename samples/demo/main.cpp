#include <iostream>
#include <iomanip>
#include <fstream>

#include "bitmap.hpp"
namespace ns = libimg;

int main(int argc, char** argv)
{
	ns::pixel_t p1(0x322432, ns::pixel_format::bpp24 | ns::pixel_format::rgb);
	ns::pixel_t p2(0x003200, ns::pixel_format::bpp24 | ns::pixel_format::rgb);
	ns::pixelptr_t p4 = &p1;
	ns::pixelref_t p3 = p2;

	std::cout << std::showbase;
	std::cout << (p1 + *p4 * 3) << std::endl;


	return 0;
}