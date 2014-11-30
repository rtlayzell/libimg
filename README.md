libimg
======

A simple modern C++ raster image library.

The library is designed to make it easy to read/write raster images of varying formats, and manipulate their contents through a consistent interface.

## Supported Formats
* .BMP - *(planned : [libbmp](http://code.google.com/p/libbmp/))*
* .TIF, .TIFF - *(planned : [unknown])*
* .PNG - *(work-in-progress : [libpng](http://www.libpng.org/pub/png/libpng.html))*
* .JPG, .JPEG - *(planned : [unknown])*


Examples
=======
(note: The current example will not build at this time, it is ment as an insight into what is to come.)

## Hello world
This sample demonstrates the most basic usage of the library and how easy it is to load any bitmap in the supported formats.
```C++
#include <iostream>

#include <bitmap.hpp>
namespace ns = libimg;

int main(int argc, char** argv)
{
	std::string fname = "image1.png";

	// load the image into a bitmap.
	ns::bitmap b(fname);

  // print some properties of the image.
	std::cout << "loaded image: " << fname << std::endl;
	std::cout << "resolution: " << b.width() << " x " << b.height() << std::endl;

	std::cout << "bit-depth: " << b.bit_depth() << std::endl;
	std::cout << "format: " << b.format() << std::endl;
}
```

## Using std.algorithms to Reverse Image Pixels.
This sample demonstrates the stl style of the library allowing the use of std.algorithms to manipulate bitmap images.
```c++
#include <iostream>
#include <fstream>
#include <vector>

#include <bitmap.hpp>
namespace ns = libimg;

// argv = ["demo.exe", "image1.bmp", "image2.png", "image3.tif", "image4.jpg"]
int main(int argc, char** argv)
{
	std::vector<ns::bitmap> bms;
	bms.reserve(argc - 1);

	// load each of the bitmaps and store them in the vector.
	for (int i = 1; i < argc; ++i)
	{
		ns::bitmap b { argv[i] };

		std::cout << "loaded image: " << argv[i] << std::endl;
		std::cout << "resolution: " << b.width() << " x " << b.height() << std::endl;

		std::cout << "bit-depth: " << b.bit_depth() << std::endl;
		std::cout << "format: " << b.format() << std::endl;

		bms.push_back(std::move(b));
	}

	// reverse the pixels of each of the bitmaps
	for (auto it = std::begin(bms); it != std::end(bms); ++it)
		std::reverse(it->begin(), it->end());

	for (int i = 1; i < argc; ++i)
	{
		std::string fname = get_new_name(argv[i]); // not an actual function.
		std::ofstream ofs(fname, std::ios::binary);

		if (!ofs)
		{
			std::cerr << "failed to open file stream for: " << fname << std::endl;
			continue;
		}

		// write the bitmap to the stream.
		bms[i].write(ofs); // or -> ofs << bms[i];
	}

	return 0;
}
```
