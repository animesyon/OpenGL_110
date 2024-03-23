/*
   Class Name:

	  CPngFile

   Description:

	  open png file

*/

#include "framework.h"
#include "pngfile.h"

// constructor
CPngFile::CPngFile()
{
	buffer = NULL;
	width = 0;
	height = 0;
	color_type = 0;
}

// destructor
CPngFile::~CPngFile()
{
	if (buffer != NULL) delete[] buffer;
}


// A description on how to use and modify libpng
// http://www.libpng.org/pub/png/libpng-1.0.3-manual.html
bool CPngFile::Open(wchar_t* szFile)
{
	errno_t err;
	FILE* fp;
	bool result = true;
	int is_not_png;
	const unsigned int number = 8;
	png_byte header[number];
	png_bytep* row_pointers;
	int rowbytes;
	png_infop end_info;
	png_structp png_ptr;
	png_infop info_ptr;

	result = true;

	// open file for reading
	if ((err = _wfopen_s(&fp, szFile, L"rb")) != 0) return false;

	// check if a file is a PNG file
	if (fread(header, 1, number, fp) != number) return false;

	is_not_png = png_sig_cmp(header, 0, number);

	if (is_not_png) {
		result = false;
		goto Close_File;
	}

	// allocate and initialize png_struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		result = false;
		goto Close_File;
	}

	// allocate and initialize png_info
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		result = false;
		goto Close_File;
	}

	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		result = false;
		goto Close_File;
	}

	// set up error handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		result = false;
		goto Close_File;
	}

	// set up the input code
	png_init_io(png_ptr, fp);

	// tell libpng that we already read a file
	png_set_sig_bytes(png_ptr, number);

	// read all the file information up to the actual image data
	png_read_info(png_ptr, info_ptr);

	// get the information from the info_ptr
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

	// bit_depth - holds the bit depth of one of the image channels
	// read only 8 bitdepth image file
	if (bit_depth != 8) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		result = false;
		goto Close_File;
	}

	// color_type - describes which color/alpha channels
	// color type can be either RGB or RGBA
	if (color_type != PNG_COLOR_TYPE_RGB && color_type != PNG_COLOR_TYPE_RGB_ALPHA) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		result = false;
		goto Close_File;
	}

	// rowbytes - number of bytes needed to hold a row
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// make it 4-byte aligned
	int rem;
	rem = (rowbytes % 4);
	rowbytes += (rem > 0 ? (4 - rem) : 0);

	// allocate memory for the image
	if (buffer != NULL) delete[] buffer;
	buffer = new png_byte[rowbytes * height];
	if (buffer == NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		result = false;
		goto Close_File;
	}

	// row_pointers is for pointing to image_data for reading the png with libpng
	row_pointers = new png_bytep[height];
	if (row_pointers == NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		result = false;
		goto Close_File;
	}

	// set the individual row_pointers to point at the correct offsets of image data
	unsigned int i;
	for (i = 0; i < height; i++)
		row_pointers[i] = buffer + i * rowbytes;

	// read the whole image
	png_read_image(png_ptr, row_pointers);

	// free all memory
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;

Close_File:

	// close file
	fclose(fp);

	return result;
}
