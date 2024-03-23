/*
   Class Name:

	  CPngFile

   Description:

	  open png file

*/

#pragma once

class CPngFile
{
	// variable
public:
	unsigned int width, height;
	int color_type;
	png_byte* buffer;

	// function
public:
	CPngFile();
	~CPngFile();

	bool Open(wchar_t* filename);
};
