#include "cMeshRenderer.h"
#include "entity.h"
#include "cCamera.h"
#include "glew.h"
#include "console.h"
#include "../DataStructure/resource_manager.h"

const int CMeshRenderer::familyId = (int)ComponentTypes::MESH_RENDERER;

CMeshRenderer::CMeshRenderer() :
	material(NULL),
	model(NULL)
{
	material = ResourceManager::getInstance().GetMaterialByName( "Default" );
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::SetModel( const char* name )
{
	model = ResourceManager::getInstance().GetModel(name);
	if (model != NULL)
	{
		SetMaterial( ResourceManager::getInstance().GetMaterialById( model->materialId ) );
	}
}

void CMeshRenderer::SetModel( ModelInstance* instance )
{
	model = instance;
}

const ModelInstance* CMeshRenderer::GetModelInstace() const
{
	return model;
}

void CMeshRenderer::SetMaterial( const char* name )
{
	material = ResourceManager::getInstance().GetMaterialByName( name );
}

void CMeshRenderer::SetMaterial( Material* mtl )
{
	material = mtl;
}

const Material* CMeshRenderer::GetMaterial() const
{
	return material;
}

void CMeshRenderer::GetUniformValues()
{
	Entity::GetComponent<CTransform>(this->entity)->GetTransform();
	Entity::GetComponent<CCamera>(this->entity)->GetViewMatrix();
	Entity::GetComponent<CCamera>(this->entity)->GetProjectionMatrix();
}