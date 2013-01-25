#include "Mesh.hpp"

//Constructor
	Mesh::Mesh(uint8_t prm, uint8_t vtxFmt)
	{
		setPrimitive(prm);
		setVertexFormat(vtxFmt);
	}
	
//Destructor
	Mesh::~Mesh()
	{
		clearMeshData();
	}
//Methods
	void Mesh::draw()
	{
		std::vector<TexVertex>::iterator it;		
		GX_Begin(GX_QUADS, GX_VTXFMT0,  meshData.size());
		for(it = meshData.begin(); it != meshData.end(); it++)
		{
			GX_Position3f32(it->x, it->y, it->z);
			GX_TexCoord2f32(it->u, it->v);		
		}
		GX_End();
	}
