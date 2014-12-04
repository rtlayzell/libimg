#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>

#include "bitmap.hpp"
namespace ns = libimg;

int main(int argc, char** argv)
{
	ns::bitmap b("image.bmp");
	std::cout << b.width() << std::endl;
	return 0;
}