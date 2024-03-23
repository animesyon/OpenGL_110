/*
   Class Name:

	  CTerrain

   Description:

	  create horizontal plane
*/

#pragma once

class CTerrain
{
private:
	int vertex_count, coord_per_vertex, stride, index_count;
	float* vertices;
	unsigned int* indices;

public:
	CTerrain();
	~CTerrain();

	void Create(float len, int div);
	void Draw();
};
