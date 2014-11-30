libimg
======

A simple modern C++ raster image library.

The library is designed to make it easy to read/write raster images of varying formats, and manipulate their contents through a consistent singular interface.

## Supported Formats
* BMP - *(planned : [libbmp](http://code.google.com/p/libbmp/))*
* PNG - *(work-in-progress : [libpng](http://www.libpng.org/pub/png/libpng.html))*
* TIF, TIFF - *(planned : [LibTIFF](http://www.remotesensing.org/libtiff/))*
* JPG, JPEG - *(planned : [libjpeg](http://libjpeg.sourceforge.net/))*

# Examples
(note: The current example will not build at this time, it is ment as an insight into what is to come.)

### Hello world
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
### Writing Bitmaps
The libraries stream manipulators `bmp`, `png`, `tif`, and `jpg` serve the purpose of specifying how to read or write a `bitmap`. By default, `bitmap` will try to deduce the file format when loading an image, and when saving a `bitmap` from scratch, the default is to produce a BMP file.

The following is an example of to load an image and re-save it in a different format.

```C++
#include <iostream>
#include <fstream>

#include <bitmap.hpp>
namespace ns = libimg;

template <typename _Fn> 
void save(std::string _Name, ns::bitmap const& _Bmp, _Fn _Manip)
{
	std::ofstream ofs(_Name, std::ios::binary);
	if (!ofs)
	{
		std::cerr << "could not open file " << _Name << " for writing.";
		return;
	}

	ofs << _Manip << _Bmp;
}

int main(int argc, char** argv)
{
	ns::bitmap img("image.bmp");

	// convert the file to other formats by specifying 
	// one of the provided stream manipulators.
	// the following would convert and save any image to a png file.
	// 
	// ofstream << libimg::png << bitmap;
	//
	save("image.png", img, ns::png);
	save("image.tif", img, ns::tif);
	save("image.jpg", img, ns::jpg);
}
```

### Using Standard Algorithms to Manipulate Image Pixels.
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
