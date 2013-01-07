#include "Mesh.h"


//Construcor
	Mesh::Mesh()
	{
		//Nozing...
	}
	
//Destructor
	Mesh::~Mesh()
	{
		clear();
	}
	
//Methods
	void Mesh::addMeshData(MeshData newMesh)
	{
		meshDataList.push_back(newMesh);
	}
	
	void Mesh::clear()
	{
		meshDataList.clear();
	}
	
	int Mesh::listSize()
	{	
		return meshDataList.size();
	}
	
	void Mesh::render()
	{
		std::vector <MeshData>::iterator meshIt;
		GX_Begin(GX_QUADS, GX_VTXFMT0, listSize());

		for(meshIt = meshDataList.begin(); meshIt != meshDataList.end(); meshIt++)
		{
			GX_Position3f32(meshIt->x, meshIt->y, meshIt->z);
			GX_Color4u8(meshIt->color.r, meshIt->color.g, meshIt->color.b, meshIt->color.a);		
		}
		GX_End();
	}



