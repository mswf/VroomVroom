#include "cMeshRenderer.h"
#include "entity.h"
#include "cCamera.h"
#include "glew.h"
#include "console.h"
#include "../DataStructure/resource_manager.h"

const int CMeshRenderer::familyId = (int)ComponentTypes::MESH_RENDERER;
std::vector< CMeshRenderer* > CMeshRenderer::list;

CMeshRenderer::CMeshRenderer() :
	material(NULL),
	model(NULL)
{
	list.push_back( this );
	material = Resources.GetMaterialByName( "Default" );
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::Call()
{
}

void CMeshRenderer::SetModel( const char* name )
{
	model = Resources.GetModel(name);
	if (model != NULL)
	{
		SetMaterial( Resources.GetMaterialById( model->materialId ) );
	}
}

void CMeshRenderer::SetModel( ModelInstance* instance )
{
	model = instance;
	if (model != NULL)
	{
		SetMaterial( Resources.GetMaterialById( model->materialId ) );
	}
}

const ModelInstance* CMeshRenderer::GetModelInstace() const
{
	return model;
}

void CMeshRenderer::SetMaterial( const char* name )
{
	material = Resources.GetMaterialByName( name );
}

void CMeshRenderer::SetMaterial( Material* mtl )
{
	material = mtl;
}

const Material* CMeshRenderer::GetMaterial() const
{
	return material;
}

std::vector< CMeshRenderer* >* CMeshRenderer::GetMeshRendererList()
{
	return &list;
}

void CMeshRenderer::GetUniformValues()
{
	Entity::GetComponent<CTransform>(this->entity)->GetTransform();
	Entity::GetComponent<CCamera>(this->entity)->GetViewMatrix();
	Entity::GetComponent<CCamera>(this->entity)->GetProjectionMatrix();
}