//
//  mImport.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 02/12/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mImport.h"
#include "../dataStructure/resource_manager.h"
#include "../dataStructure/shader.h"

void mImport::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");

	lua_pushcfunction(L, lw_importModel__);
	lua_setfield(L, -2, "importModel");

	//lua_pushcfunction(L, lw_importMesh__);
	//lua_setfield(L, -2, "importMesh");

	//lua_pushcfunction(L, lw_importMaterial__);
	//lua_setfield(L, -2, "importMaterial");

	lua_pushcfunction(L, lw_importTexture__);
	lua_setfield(L, -2, "importTexture");

	lua_pushcfunction(L, lw_reloadTexture__);
	lua_setfield(L, -2, "reloadTexture");

	lua_pushcfunction(L, lw_reloadShader__);
	lua_setfield(L, -2, "reloadShader");
	
	lua_pushcfunction(L, lw_isModelLoaded__);
	lua_setfield(L, -2, "isModelLoaded");
	
	lua_pushcfunction(L, lw_isTextureLoaded__);
	lua_setfield(L, -2, "isTextureLoaded");
	
	lua_pushcfunction(L, lw_isMaterialLoaded__);
	lua_setfield(L, -2, "isMaterialLoaded");
	
	lua_pushcfunction(L, lw_isShaderLoaded__);
	lua_setfield(L, -2, "isShaderLoaded");
}

lFuncImp(mImport, importModel)
{
	lua_settop(L, 1);
	lgString(path, 1, "undefined");

	Assets.ImportMesh(path.c_str());

	return 0;
}


lFuncImp(mImport, importTexture)
{
	lua_settop(L, 2);
	lgString(path, 1, "undefined");
	lgBool(flipped, 2, false);

	Assets.ImportImage(path.c_str(), flipped);

	return 0;
}

lFuncImp(mImport, reloadTexture)
{
	lua_settop(L, 2);
	lgString(path, 1, "undefined");
	lgBool(flipped, 1, false);

	Assets.ReImportImage(path.c_str(), flipped);

	return 0;
}

lFuncImp(mImport, reloadShader)
{
	lua_settop(L, 2);
	lgString(path, 1, "undefined");
	lgString(type, 2, "none");

	GLSLShaderType shaderType = GLSLShaderType::NONE;
	if (type == "VERTEX")
	{
		shaderType = GLSLShaderType::VERTEX;
	}
	else if (type == "GEOMETRY")
	{
		shaderType = GLSLShaderType::GEOMETRY;
	}
	else if (type == "FRAGMENT")
	{
		shaderType = GLSLShaderType::FRAGMENT;
	}
	else if (type == "TESS_CONTROL")
	{
		shaderType = GLSLShaderType::TESS_CONTROL;
	}
	else if (type == "TESS_EVALUATION")
	{
		shaderType = GLSLShaderType::TESS_EVALUATION;
	}

	Assets.ReImportShader(path.c_str(), shaderType);

	return 0;
}

lFuncImp(mImport, isModelLoaded)
{
	lua_settop(L,1);
	lgString(name, 1, "undefined");
	bool exists = Assets.MeshExists(name.c_str());
	lua_pushboolean(L, exists);
	return 1;
}

lFuncImp(mImport, isTextureLoaded)
{
	lua_settop(L,1);
	lgString(name, 1, "undefined");
	bool exists = Assets.ImageExists(name.c_str());
	lua_pushboolean(L, exists);
	return 1;
}

lFuncImp(mImport, isMaterialLoaded)
{
	lua_settop(L,1);
	lgString(name, 1, "undefined");
	bool exists = Assets.MaterialExists(name.c_str());
	lua_pushboolean(L, exists);
	return 1;
}

lFuncImp(mImport, isShaderLoaded)
{
	lua_settop(L,1);
	lgString(name, 1, "undefined");
	bool exists = Assets.ShaderObjectExists(name.c_str());
	lua_pushboolean(L, exists);
	return 1;
}