#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <Math_utils.h>

typedef struct
{
	float x, y, z;
	Color4u8 color;
	//Texture?
}MeshData;


class Mesh
{
public:
//Construcor
	Mesh();
//Destructor
	~Mesh();
//Methods
	void addMeshData(MeshData newMesh);
	void clear();
	int listSize();
	void render();
//Variables
	std::vector <MeshData> meshDataList; 
};


#endif
