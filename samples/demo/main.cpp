#include <iostream>
#include <fstream>

#include <bitmap.hpp>
namespace ns = libimg;

int main(int argc, char** argv)
{
	ns::bitmap img("bigimage.png");
	img.write(std::cout << ns::png);
}