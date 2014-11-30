#include <iostream>
#include <fstream>

#include <bitmap.hpp>
namespace ns = libimg;

int main(int argc, char** argv)
{
	ns::bitmap img("rocket.bmp");
	std::cout << img.width() << " x " << img.height() << std::endl;
}