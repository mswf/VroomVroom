#ifndef cMeshRenderer_h
#define cMeshRenderer_h

#include "component.h"
#include "../DataStructure/material.h"
#include "cMeshFilter.h"

class CMeshRenderer : public Component
{
	public:
	
		static const int familyId;
		CMeshRenderer();
		~CMeshRenderer();
		void Update();
		void SetVertexAttributes( const char* attribute );
		void SetMaterial( Material* material );
		void SetMeshFilter( CMeshFilter* mesh );
	
		CMeshFilter* meshFilter;
		Material* material;
	
	private:
	
};

#endif /* cMeshRenderer_h */
