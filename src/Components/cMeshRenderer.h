#ifndef cMeshRenderer_h
#define cMeshRenderer_h

#include "component.h"
#include "../DataStructure/material.h"
#include "../DataStructure/data_types.h"

class CMeshRenderer : public Component
{
	public:
	
		static const int familyId;

		CMeshRenderer();
		~CMeshRenderer();
	
		void SetModel( const char* name );
		void SetMaterial( const char* name );
		void SetMaterial( Material* mtl );
	
		void GetUniformValues();
		//void Buffer( const Mesh* m, ModelInstance* instance );
		//void UnBuffer();
		ModelInstance* model;
		Material* material;
		
	
	private:
	
		//Material* material;
		//ModelInstance* model;
};

#endif /* cMeshRenderer_h */
