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
		void SetModel( ModelInstance* instance );
		void SetMaterial( const char* name );
		void SetMaterial( Material* mtl );
	
		void GetUniformValues();
		static std::vector< CMeshRenderer* >* GetMeshRendererList();
		const ModelInstance* GetModelInstace() const;
		const Material* GetMaterial() const;
	
	private:
	
		Material* material;
		ModelInstance* model;
		static std::vector< CMeshRenderer* > list;
};

#endif /* cMeshRenderer_h */
