/*
   Class Name:

	  CMd2File

   Description:

	  open md2 file

*/

#pragma once

// header
typedef struct
{
	char 	id[4]; 					// Must be equal to "IDP2"
	int 	version; 				// MD2 version
	int 	texture_width; 			// Width of the texture
	int 	texture_height;			// Height of the texture
	int 	framesize; 				// Size of one frame in bytes
	int 	texture_image_count; 	// Number of textures
	int 	vertex_count; 			// Number of vertices
	int 	texture_count;			// Number of texture coordinates
	int 	face_count; 			// Number of triangles
	int 	cmd_count; 				// Number of OpenGL commands
	int 	frame_count; 			// Number of frames
	int 	texture_name_offset;	// Offset to skin names
	int 	texture_offset; 		// Offset to s-t texture coordinates
	int 	face_offset; 			// Offset to triangles
	int 	frame_offset; 			// Offset to frame data
	int 	cmd_offset; 			// Offset to OpenGL commands
	int 	end_offset; 			// Offset to end of file
}MD2FILEHEADER;

// data structure for texture
typedef struct
{
	short s;
	short t;
}TEXTURE_STRUCT;

// data structure for face
typedef struct
{
	unsigned short VertexIndex[3];
	unsigned short TextureIndex[3];
}FACE_STRUCT;

// data structure for vertex
typedef struct
{
	unsigned char vertex[3];
	unsigned char normal;
}VERTEX_STRUCT;

// data structure for frame
typedef struct
{
	float scale[3];
	float translate[3];
	char name[16];
	VERTEX_STRUCT data[1];
}FRAME_STRUCT;

// data structure to draw model
typedef struct
{
	float nx, ny, nz;
	float s1, t1, s2, t2, s3, t3;
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
}MD2_STRUCT;

class CMd2File
{
private:
	MD2FILEHEADER* header;
	unsigned char* buffer;
	FRAME_STRUCT* frame;
	FACE_STRUCT* face;
	TEXTURE_STRUCT* st;
	MD2_STRUCT data;

public:

	CMd2File();
	~CMd2File();

	bool Open(wchar_t* filename);

	void SetFrame(int index);

	int GetFaceCount();
	int GetFrameCount();
	void GetTextureName(char* str, size_t n);

	MD2_STRUCT& operator[](int i);
};
