#ifndef data_types_h
#define data_types_h

#include "../Utilities/typedef.h"
#include <vector>
#include <map>
#include <string>
#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"

enum class FilterType
{
	NEAREST,
	LINEAR,
	NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR
};

enum class WrapType
{
	CLAMP_EDGE,
	MIRROR_CLAMP_EDGE,
	CLAMP_BORDER,
	REPEAT,
	MIRRORED_REPEAT
};

struct ShaderProgram;

struct Subroutines
{
	std::map< std::string, unsigned int > locations;
	uint32* subroutines;
	uint32 numSubroutines = 0;
};

struct ShaderObject
{
	uint32 shader = 0;
	uint32 shaderType = 0;
	// Create by the number of subroutine uniforms & clean up when finished
	//TODO(Valentinas): Use std::Array instead of pointer to memory block
	Subroutines subroutine;
	bool updateProgram = true;
	ShaderProgram* program;
};

struct ShaderProgram
{
	uint32 program = 0;
	string name = "No name";
	// Create by the number of shader objects & clean up when finished
	std::vector< ShaderObject* > shaders;
	bool separate = false;
};

struct Mesh
{
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3 > bitangents;
	std::vector< glm::vec3 > tangents;
	std::vector< uint32 > indices;
	uint32 numIndices = 0;
	uint32 materialId = 0;
	float scaleFactor = 1;
	bool isBuffered = false;
	bool hasBufferChanged = false;
	bool hasPositions = false;
	bool hasNormals = false;
	bool hasTangentsAndBitangets = false;
	bool hasUVs = false;
};

struct ModelInstance
{
	uint32 vao = 0;
	uint32 vbo = 0;
	uint32 numIndices = 0;
	uint32 indiceBuffer = 0;
	uint32 materialId = 0;
};

struct ImageData
{
	int32 imageId = -1;
	uint8* pixelData;
	uint32 width = 0;
	uint32 height = 0;
	uint32 components = 0;
	FilterType minFilter;
	FilterType magFilter;
	WrapType wrap;
	bool isBuffered = false;
	bool mipmapping = false;
};

#endif /* data_types_h */
