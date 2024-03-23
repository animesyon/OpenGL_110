/*
   Class Name:

	  CTerrain

   Description:

	  create horizontal plane
*/

#include "framework.h"
#include "terrain.h"

// constructor
CTerrain::CTerrain()
{
	vertex_count = 0;
	coord_per_vertex = 3;                        // x, y, z
	stride = coord_per_vertex * sizeof(float);   // in bytes
	vertices = NULL;
	indices = NULL;
}

// destructor
CTerrain::~CTerrain()
{
	if (vertices != NULL) delete[] vertices;
	if (indices != NULL) delete[] indices;
}

//
void CTerrain::Create(float len, int div)
{
	int i, j, k, n, size, i1, i2, i3, i4;
	float upd, x, y, z, so;

	so = -len / 2.0f;

	upd = len / (float)div;
	n = div;

	//                                                
	//            0   1   2   3   ...    n-3 n-2 n-1  n
	//          0 +---+---+---+--- ... ---+---+---+---+
	//            |   |   |   |           |   |   |   |
	//          1 +---+---+---+--- ... ---+---+---+---+
	//            |   |   |   |           |   |   |   |
	//            .   .   .   .           .   .   .   .
	//            .   .   .   .           .   .   .   .
	//            .   .   .   .           .   .   .   .
	//            .   .   .   .           .   .   .   .
	//            |   |   |   |           |   |   |   |
	//        n-1 +---+---+---+--- ... ---+---+---+---+
	//            |   |   |   |           |   |   |   |
	//          n +---+---+---+--- ... ---+---+---+---+

	vertex_count = (n + 1) * (n + 1);
	size = vertex_count * coord_per_vertex;
	vertices = new float[size];

	k = 0;
	y = 0.0f;

	z = so;

	for (i = 0; i < (n + 1); i++)
	{
		x = so;

		for (j = 0; j < (n + 1); j++)
		{
			i1 = i;
			i2 = i1 + 1;
			i3 = 0;
			i4 = i3 + 1;

			vertices[k++] = x;
			vertices[k++] = y;
			vertices[k++] = z;

			x += upd;
		}

		z += upd;
	}

	// 
	//       i1        i2
	//       +---------+
	//       |         |
	//       |         |
	//       |         |
	//       +---------+
	//       i3        i4

	index_count = 4 * n * n;
	indices = new unsigned int[index_count];

	k = 0;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			i1 = i * (n + 1) + j;
			i2 = i1 + 1;
			i3 = i1 + n + 1;
			i4 = i3 + 1;

			indices[k++] = i1;
			indices[k++] = i2;
			indices[k++] = i4;
			indices[k++] = i3;
		}
	}
}

//
void CTerrain::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(coord_per_vertex, GL_FLOAT, stride, (GLvoid*)vertices);
	glDrawElements(GL_QUADS, index_count, GL_UNSIGNED_INT, (GLvoid*)indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//
