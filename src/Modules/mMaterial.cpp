//
//  mMaterial.cpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 30/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#include "mMaterial.h"

#include "../console.h"
#include "../DataStructure/resource_manager.h"
#include "../DataStructure/material.h"

void mMaterial::Bind(lua_State* L)
{
	lua_getglobal(L, "Engine");
	lua_pushcfunction(L, lw_getMaterial__);
	lua_setfield(L, -2, "getMaterial");
	
	
	lua_newtable(L);
	lStart(BaseMaterial)
		lBind(__engineInit)
		lBind(setShader)
		lBind(setDiffuseTexture)
		lBind(setSpecularTexture)
		lBind(setNormalTexture)
		lBind(setCubeMapTexture)
		lBind(setHeightMapTexture)
	
		lBind(setDrawingWireframe)
		lBind(setTwoSided)
		
		lBind(setShininess)
		lBind(setShininessStrength)
		lBind(setOpacity)
		lBind(setBumpScaling)
		
		lBind(setAmbientColor)
		lBind(setDiffuseColor)
		lBind(setSpecularColor)
		lBind(setEmissiveColor)
		lBind(setTransparentColor)
		lBind(setReflectiveColor)
		
		lBind(getDrawingWireframe)
		lBind(getTwoSided)
		
		lBind(getShininess)
		lBind(getShininessStrength)
		lBind(getOpacity)
		lBind(getBumpScaling)
		
		lBind(getAmbientColor)
		lBind(getDiffuseColor)
		lBind(getSpecularColor)
		lBind(getEmissiveColor)
		lBind(getTransparentColor)
		lBind(getReflectiveColor)
	lEnd(BaseMaterial)
	luaL_openlib(L, 0, BaseMaterial_funcs, 0);
	lua_setfield(L, -2, "baseMaterial");

}

lFuncImp(mMaterial, getMaterial)
{
	lua_settop(L, 1);
	
	if(!lua_isstring(L, 1))
	{
		Terminal.Warning("Invalid parameter for Engine.getMaterial");
		return 0;
	}
	const char* name = lua_tostring(L, 1);
	
	
	lua_newtable(L);
	
	Material* mat = ResourceManager::getInstance().GetMaterialByName(name);
	
	lua_pushlightuserdata(L, mat);
	lua_setfield(L, -2, "__coreMaterial__");
	
	lua_getglobal(L, "Engine");
	lua_newtable(L);
	lua_getfield(L, -2, "baseMaterial");
	lua_setfield(L, -2, "__index");
	lua_setmetatable(L, 2);
	lua_settop(L, 2);
	return 1;
}

lFuncImp(mMaterial, __engineInit)
{
	lua_settop(L, 1);
	
	Material* mat = new Material();
	
	lua_pushlightuserdata(L, mat);
	lua_setfield(L, -2, "__coreMaterial__");
	return 0;
}

lFuncImp(mMaterial, setShader)
{
	lua_settop(L, 2);
	lua_getfield(L, 1, "__coreMaterial__");
	Material* mat = (Material*)lua_touserdata(L, -1);

	//TODO(bobn): make this
	return 0;
}



lBindSetMatTex(setDiffuseTexture, SetDiffuseTexture);
lBindSetMatTex(setSpecularTexture, SetSpecularTexture);
lBindSetMatTex(setNormalTexture, SetNormalTexture);
lBindSetMatTex(setCubeMapTexture, SetCubeMapTexture);
lBindSetMatTex(setHeightMapTexture, SetHeightMapTexture);

lBindSetMatBool(setDrawingWireframe, SetDrawingWireframe);
lBindSetMatBool(setTwoSided, SetTwoSided);

lBindSetMatFloat(setShininess, SetShininess);
lBindSetMatFloat(setShininessStrength, SetShininessStrength);
lBindSetMatFloat(setOpacity, SetOpacity);
lBindSetMatFloat(setBumpScaling, SetBumpScaling);

lBindSetMatCol(setAmbientColor, SetAmbientColor);
lBindSetMatCol(setDiffuseColor, SetDiffuseColor);
lBindSetMatCol(setSpecularColor, SetSpecularColor);
lBindSetMatCol(setEmissiveColor, SetEmissiveColor);
lBindSetMatCol(setTransparentColor, SetTransparentColor);
lBindSetMatCol(setReflectiveColor, SetReflectiveColor);


lBindGetMatBool(getDrawingWireframe, GetDrawingWireframe);
lBindGetMatBool(getTwoSided, GetTwoSided);

lBindGetMatFloat(getShininess, GetShininess);
lBindGetMatFloat(getShininessStrength, GetShininessStrength);
lBindGetMatFloat(getOpacity, GetOpacity);
lBindGetMatFloat(getBumpScaling, GetBumpScaling);

lBindGetMatCol(getAmbientColor, GetAmbientColor);
lBindGetMatCol(getDiffuseColor, GetDiffuseColor);
lBindGetMatCol(getSpecularColor, GetSpecularColor);
lBindGetMatCol(getEmissiveColor, GetEmissiveColor);
lBindGetMatCol(getTransparentColor, GetTransparentColor);
lBindGetMatCol(getReflectiveColor, GetReflectiveColor);

