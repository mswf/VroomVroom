//
//  mMaterial.hpp
//  VroomVroom
//
//  Created by Robin Zaagsma on 30/11/15.
//  Copyright © 2015 Valentinas Rimeika. All rights reserved.
//

#ifndef mMaterial_hpp
#define mMaterial_hpp

#include <stdio.h>
#include "moduleMacros.h"

class mMaterial
{
	public:
		static void Bind(lua_State*);
	
	private:
		mMaterial();
		~mMaterial();
	
		lFuncDef(getMaterial);
	
		lFuncDef(__engineInit);

		lFuncDef(setShader);
		lFuncDef(makeQuadShader);
	
		lFuncDef(setDiffuseTexture);
		lFuncDef(setSpecularTexture);
		lFuncDef(setNormalTexture);
		lFuncDef(setCubeMapTexture);
		lFuncDef(setHeightMapTexture);
	
	
	
		lFuncDef(setDrawingWireframe);
		lFuncDef(setTwoSided);
	
		lFuncDef(setShininess);
		lFuncDef(setShininessStrength);
		lFuncDef(setOpacity);
		lFuncDef(setBumpScaling);
	
		lFuncDef(setAmbientColor);
		lFuncDef(setDiffuseColor);
		lFuncDef(setSpecularColor);
		lFuncDef(setEmissiveColor);
		lFuncDef(setTransparentColor);
		lFuncDef(setReflectiveColor);
	
	
	
		lFuncDef(getDrawingWireframe);
		lFuncDef(getTwoSided);
		
		lFuncDef(getShininess);
		lFuncDef(getShininessStrength);
		lFuncDef(getOpacity);
		lFuncDef(getBumpScaling);
	
		lFuncDef(getAmbientColor);
		lFuncDef(getDiffuseColor);
		lFuncDef(getSpecularColor);
		lFuncDef(getEmissiveColor);
		lFuncDef(getTransparentColor);
		lFuncDef(getReflectiveColor);

};

//macros for binding in the cpp

#define lBindSetMatTex(NAME, FUNC)\
	lFuncImp(mMaterial, NAME)\
	{\
		lua_settop(L, 2);\
		lua_getfield(L, 1, "__coreMaterial__");\
		Material* mat = (Material*)lua_touserdata(L, -1);\
	\
		mat->FUNC(lua_tostring(L,2));\
	\
		return 0;\
	}\

#define lBindSetMatBool(NAME, FUNC)\
	lFuncImp(mMaterial, NAME)\
	{\
		lua_settop(L, 2);\
		lua_getfield(L, 1, "__coreMaterial__");\
		Material* mat = (Material*)lua_touserdata(L, -1);\
	\
		mat->FUNC(lua_toboolean(L, 2));\
	\
		return 0;\
	}\

#define lBindSetMatFloat(NAME, FUNC)\
	lFuncImp(mMaterial, NAME)\
	{\
		lua_settop(L, 2);\
		lua_getfield(L, 1, "__coreMaterial__");\
		Material* mat = (Material*)lua_touserdata(L, -1);\
	\
		mat->FUNC(lua_tonumber(L, 2));\
	\
		return 0;\
	}\

#define lBindSetMatCol(NAME, FUNC)\
	lFuncImp(mMaterial, NAME)\
	{\
		lua_settop(L, 5);\
		lua_getfield(L, 1, "__coreMaterial__");\
		Material* mat = (Material*)lua_touserdata(L, -1);\
	\
		glm::vec4 col;\
		col.r = lua_tonumber(L, 2);\
		col.g = lua_tonumber(L, 3);\
		col.b = lua_tonumber(L, 4);\
		col.a = lua_tonumber(L, 5);\
	\
		mat->FUNC(col);\
	\
		return 0;\
	}\

#define lBindGetMatBool(NAME, FUNC)\
	lFuncImp(mMaterial, NAME)\
	{\
		lua_settop(L, 1);\
		lua_getfield(L, 1, "__coreMaterial__");\
		Material* mat = (Material*)lua_touserdata(L, -1);\
	\
		lua_pushboolean(L, mat->FUNC());\
	\
		return 1;\
	}\

#define lBindGetMatFloat(NAME, FUNC)\
	lFuncImp(mMaterial, NAME)\
	{\
		lua_settop(L, 1);\
		lua_getfield(L, 1, "__coreMaterial__");\
		Material* mat = (Material*)lua_touserdata(L, -1);\
	\
		lua_pushnumber(L, mat->FUNC());\
	\
		return 1;\
	}\

#define lBindGetMatCol(NAME, FUNC)\
	lFuncImp(mMaterial, NAME)\
	{\
		lua_settop(L, 1);\
		lua_getfield(L, 1, "__coreMaterial__");\
		Material* mat = (Material*)lua_touserdata(L, -1);\
	\
		glm::vec4 col =  mat->FUNC();\
	\
		lua_pushnumber(L, col.r);\
		lua_pushnumber(L, col.g);\
		lua_pushnumber(L, col.b);\
		lua_pushnumber(L, col.a);\
	\
		return 4;\
	}\

#endif /* mMaterial_hpp */
