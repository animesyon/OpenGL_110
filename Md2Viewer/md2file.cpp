/*
   Class Name:

	  CMd2File

   Description:

	  open md2 file

*/

#include "framework.h"
#include "md2file.h"

// constructor
CMd2File::CMd2File()
{
	buffer = NULL;
}

// destructor
CMd2File::~CMd2File()
{
	if (buffer != NULL) {
		delete[] buffer;
		buffer = NULL;
	}
}

//
bool CMd2File::Open(wchar_t* filename)
{
	FILE* fp;
	errno_t err;
	char id[4];
	long n;

	// open file
	if ((err = _wfopen_s(&fp, filename, L"rb")) != 0)
		return false;

	// read id
	if (fread(id, sizeof(char), 4, fp) != 4) {
		fclose(fp);
		return false;
	}

	// check if a file is a MD2 file
	if (!(id[0] == 'I' && id[1] == 'D' && id[2] == 'P' && id[3] == '2')) {
		fclose(fp);
		return false;
	}

	// get file size
	fseek(fp, 0, SEEK_END);
	n = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// allocate memory
	if (buffer != NULL) delete[] buffer;
	buffer = new unsigned char[n];

	// error checking
	if (!buffer) return false;

	// read the whole file
	if (fread(buffer, sizeof(unsigned char), n, fp) != n) {
		fclose(fp);
		return false;
	}

	// initialize variables
	header = (MD2FILEHEADER*)buffer;
	face = (FACE_STRUCT*)&buffer[header->face_offset];
	st = (TEXTURE_STRUCT*)&buffer[header->texture_offset];

	// go to first frame
	SetFrame(0);

	// close file
	fclose(fp);

	return true;
}

// set the frame with index index
void CMd2File::SetFrame(int index)
{
	frame = (FRAME_STRUCT*)&buffer[header->frame_offset + header->framesize * index];
}

// return the number of triangles in the model
int CMd2File::GetFaceCount()
{
	return (buffer == NULL ? 0 : header->face_count);
}

// return the number of frames of animation
int CMd2File::GetFrameCount()
{
	return header->frame_count;
}

// return the name of a texture file for the model
void CMd2File::GetTextureName(char* str, size_t n)
{
	char* p;

	p = (char*)&buffer[header->texture_name_offset];
	strcpy_s(str, n, p);
}

// return the data we need to draw the model
// we swap the y and z to make the model standing up
MD2_STRUCT& CMd2File::operator[](int i)
{
	data.s1 = (float)st[face[i].TextureIndex[0]].s / (float)header->texture_width;
	data.t1 = (float)st[face[i].TextureIndex[0]].t / (float)header->texture_height;

	data.s2 = (float)st[face[i].TextureIndex[1]].s / (float)header->texture_width;
	data.t2 = (float)st[face[i].TextureIndex[1]].t / (float)header->texture_height;

	data.s3 = (float)st[face[i].TextureIndex[2]].s / (float)header->texture_width;
	data.t3 = (float)st[face[i].TextureIndex[2]].t / (float)header->texture_height;

	data.nx = frame->data[face[i].VertexIndex[0]].normal;
	data.nz = frame->data[face[i].VertexIndex[1]].normal;
	data.ny = frame->data[face[i].VertexIndex[2]].normal;

	data.x1 = frame->scale[0] * frame->data[face[i].VertexIndex[0]].vertex[0] + frame->translate[0];
	data.z1 = frame->scale[1] * frame->data[face[i].VertexIndex[0]].vertex[1] + frame->translate[1];
	data.y1 = frame->scale[2] * frame->data[face[i].VertexIndex[0]].vertex[2] + frame->translate[2];

	data.x2 = frame->scale[0] * frame->data[face[i].VertexIndex[1]].vertex[0] + frame->translate[0];
	data.z2 = frame->scale[1] * frame->data[face[i].VertexIndex[1]].vertex[1] + frame->translate[1];
	data.y2 = frame->scale[2] * frame->data[face[i].VertexIndex[1]].vertex[2] + frame->translate[2];

	data.x3 = frame->scale[0] * frame->data[face[i].VertexIndex[2]].vertex[0] + frame->translate[0];
	data.z3 = frame->scale[1] * frame->data[face[i].VertexIndex[2]].vertex[1] + frame->translate[1];
	data.y3 = frame->scale[2] * frame->data[face[i].VertexIndex[2]].vertex[2] + frame->translate[2];

	return data;
}

//
